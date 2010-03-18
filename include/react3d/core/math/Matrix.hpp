#ifndef REACT3D_MATRIX__HPP
#define REACT3D_MATRIX__HPP

////////////////////////////////////////////////////////////////////////////////

#include <react3d/core/math/Matrix.h>

////////////////////////////////////////////////////////////////////////////////

/******************************************
  Matrix inversion technique:
Given a matrix mat, we want to invert it.
mat = [ r00 r01 r02 a
        r10 r11 r12 b
        r20 r21 r22 c
        tx  ty  tz  d ]
We note that this matrix can be split into three matrices.
mat = rot * trans * corr, where rot is rotation part, trans is translation part,
   and corr is the correction due to perspective (if any).
rot = [ r00 r01 r02 0
        r10 r11 r12 0
        r20 r21 r22 0
        0   0   0   1 ]
trans = [ 1  0  0  0
          0  1  0  0
          0  0  1  0
          tx ty tz 1 ]
corr = [ 1 0 0 px
         0 1 0 py
         0 0 1 pz
         0 0 0 s ]
where the elements of corr are obtained from linear combinations of the elements
   of rot, trans, and mat.
So the inverse is mat' = (trans * corr)' * rot', where rot' must be computed the
   traditional way, which is easy since it is only a 3x3 matrix.
This problem is simplified if [px py pz s] = [0 0 0 1], which will happen if mat
   was composed only of rotations, scales, and translations (which is common).
   In this case, we can ignore corr entirely which saves on a lot of computations.
******************************************/

template <typename T> void Matrix4x4<T>::SetRotate(const Vec4<T>& quat)
{
   Vec4<T> quatNormalized(~quat);

   // Source: Gamasutra, Rotating Objects Using Quaternions
   //
   //http://www.gamasutra.com/features/19980703/quaternions_01.htm

   T wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

   // calculate coefficients
   x2 = quatNormalized[X] + quatNormalized[X];
   y2 = quatNormalized[Y] + quatNormalized[Y];
   z2 = quatNormalized[Z] + quatNormalized[Z];

   xx = quatNormalized[X] * x2;
   xy = quatNormalized[X] * y2;
   xz = quatNormalized[X] * z2;

   yy = quatNormalized[Y] * y2;
   yz = quatNormalized[Y] * z2;
   zz = quatNormalized[Z] * z2;

   wx = quatNormalized[W] * x2;
   wy = quatNormalized[W] * y2;
   wz = quatNormalized[W] * z2;

   // Note.  Gamasutra gets the matrix assignments inverted, resulting
   // in left-handed rotations, which is contrary to OpenGL and OSG's 
   // methodology.  The matrix assignment has been altered in the next
   // few lines of code to do the right thing.
   // Don Burns - Oct 13, 2001
   (*this)(0, 0) = T(1) - (yy + zz);
   (*this)(1, 0) = xy - wz;
   (*this)(2, 0) = xz + wy;


   (*this)(0, 1) = xy + wz;
   (*this)(1, 1) = T(1) - (xx + zz);
   (*this)(2, 1) = yz - wx;

   (*this)(0, 2) = xz - wy;
   (*this)(1, 2) = yz + wx;
   (*this)(2, 2) = T(1) - (xx + yy);
}

template <typename T> Vec4<T> Matrix4x4<T>::GetRotate() const
{
   Vec4<T> q;
   T       s;
   T       tq[4];
   int     i, j;

   // Use tq to store the largest trace
   tq[0] = 1 + (*this)(0, 0) + (*this)(1, 1) + (*this)(2, 2);
   tq[1] = 1 + (*this)(0, 0) - (*this)(1, 1) - (*this)(2, 2);
   tq[2] = 1 - (*this)(0, 0) + (*this)(1, 1) - (*this)(2, 2);
   tq[3] = 1 - (*this)(0, 0) - (*this)(1, 1) + (*this)(2, 2);

   // Find the maximum (could also use stacked if's later)
   j = 0;
   for (i = 1; i < 4; ++i) { j = (tq[i] > tq[j]) ? i : j; }

   // check the diagonal
   if (j == 0)
   {
      /* perform instant calculation */
      q[W] = tq[0];
      q[X] = (*this)(1, 2) - (*this)(2, 1); 
      q[Y] = (*this)(2, 0) - (*this)(0, 2); 
      q[Z] = (*this)(0, 1) - (*this)(1, 0); 
   }
   else if (j==1)
   {
      q[W] = (*this)(1, 2) - (*this)(2, 1); 
      q[X] = tq[1];
      q[Y] = (*this)(0, 1) + (*this)(1, 0); 
      q[Z] = (*this)(2, 0) + (*this)(0, 2); 
   }
   else if (j==2)
   {
      q[W] = (*this)(2, 0) - (*this)(0, 2); 
      q[X] = (*this)(0, 1) + (*this)(1, 0); 
      q[Y] = tq[2];
      q[Z] = (*this)(1, 2) + (*this)(2, 1); 
   }
   else /* if (j==3) */
   {
      q[W] = (*this)(0, 1) - (*this)(1, 0); 
      q[X] = (*this)(2, 0) + (*this)(0, 2); 
      q[Y] = (*this)(1, 2) + (*this)(2, 1); 
      q[Z] = tq[3];
   }

   s = sqrtf(0.25f/tq[j]);
   q[W] *= s;
   q[X] *= s;
   q[Y] *= s;
   q[Z] *= s;

   return q;
}

template <typename T> bool Matrix4x4<T>::Invert()
{
   bool is_4x3 = (*this)(0, 3) == T(0)
              && (*this)(1, 3) == T(0)
              && (*this)(2, 3) == T(0)
              && (*this)(3, 3) == T(1);
   return is_4x3 ? Invert4x3() : Invert4x4();
}

template <typename T> Matrix4x4<T> Matrix4x4<T>::Inverse() const
{
   Matrix4x4 out(*this);

   bool success = out.Invert();

   return out;
}

template <typename T> bool Matrix4x4<T>::Invert4x3()
{
   Matrix4x4<T>& self = *this;

   register T r00, r01, r02,
              r10, r11, r12,
              r20, r21, r22;
   // Copy rotation components directly into registers for speed
   r00 = self(0, 0); r01 = self(0, 1); r02 = self(0, 2);
   r10 = self(1, 0); r11 = self(1, 1); r12 = self(1, 2);
   r20 = self(2, 0); r21 = self(2, 1); r22 = self(2, 2);

   // Partially compute inverse of rot
   self(0, 0) = r11*r22 - r12*r21;
   self(0, 1) = r02*r21 - r01*r22;
   self(0, 2) = r01*r12 - r02*r11;

   // Compute determinant of rot from 3 elements just computed
   register T one_over_det = T(1) / (r00 * self(0, 0) + r10 * self(0, 1) + r20 * self(0, 2));
   r00 *= one_over_det; r10 *= one_over_det; r20 *= one_over_det;  // Saves on later computations

   // Finish computing inverse of rot
   self(0, 0) *= one_over_det;
   self(0, 1) *= one_over_det;
   self(0, 2) *= one_over_det;
   self(0, 3) = 0.0;
   self(1, 0) = r12*r20 - r10*r22; // Have already been divided by det
   self(1, 1) = r00*r22 - r02*r20; // same
   self(1, 2) = r02*r10 - r00*r12; // same
   self(1, 3) = 0.0;
   self(2, 0) = r10*r21 - r11*r20; // Have already been divided by det
   self(2, 1) = r01*r20 - r00*r21; // same
   self(2, 2) = r00*r11 - r01*r10; // same
   self(2, 3) = 0.0;
   self(3, 3) = 1.0;

   // We no longer need the rxx or det variables anymore, so we can reuse them for whatever we want.  But we will still rename them for the sake of clarity.

#define d r22
   d = self(3, 3);

   if ((d - T(1))*(d - T(1)) > T(1.0e-6))  // Involves perspective, so we must
   {                       // compute the full inverse

      Matrix4x4 TPinv;
      self(3, 0) = self(3, 1) = self(3, 2) = 0.0;

#define px r00
#define py r01
#define pz r02
#define one_over_s  one_over_det
#define a  r10
#define b  r11
#define c  r12

      a  = self(0, 3);  b = self(1, 3);  c = self(2, 3);
      px = self(0, 0) * a + self(0, 1) * b + self(0, 2)*c;
      py = self(1, 0) * a + self(1, 1) * b + self(1, 2)*c;
      pz = self(2, 0) * a + self(2, 1) * b + self(2, 2)*c;

#undef a
#undef b
#undef c
#define tx r10
#define ty r11
#define tz r12

      tx = self(3, 0); ty = self(3, 1); tz = self(3, 2);
      one_over_s  = T(1) / (d - (tx*px + ty*py + tz*pz));

      tx *= one_over_s; ty *= one_over_s; tz *= one_over_s;  // Reduces number of calculations later on

      // Compute inverse of trans*corr
      TPinv(0, 0) = tx*px + T(1);
      TPinv(0, 1) = ty*px;
      TPinv(0, 2) = tz*px;
      TPinv(0, 3) = -px * one_over_s;
      TPinv(1, 0) = tx*py;
      TPinv(1, 1) = ty*py + T(1);
      TPinv(1, 2) = tz*py;
      TPinv(1, 3) = -py * one_over_s;
      TPinv(2, 0) = tx*pz;
      TPinv(2, 1) = ty*pz;
      TPinv(2, 2) = tz*pz + T(1);
      TPinv(2, 3) = -pz * one_over_s;
      TPinv(3, 0) = -tx;
      TPinv(3, 1) = -ty;
      TPinv(3, 2) = -tz;
      TPinv(3, 3) = one_over_s;

      // Finish computing full inverse of mat
      self = TPinv * self;

#undef px
#undef py
#undef pz
#undef one_over_s
#undef d
   }
   else // Rightmost column is [0; 0; 0; 1] so it can be ignored
   {
      tx = self(3, 0); ty = self(3, 1); tz = self(3, 2);

      // Compute translation components of mat'
      self(3, 0) = -(tx*self(0, 0) + ty*self(1, 0) + tz*self(2, 0));
      self(3, 1) = -(tx*self(0, 1) + ty*self(1, 1) + tz*self(2, 1));
      self(3, 2) = -(tx*self(0, 2) + ty*self(1, 2) + tz*self(2, 2));

#undef tx
#undef ty
#undef tz
   }

   return true;
}

template <class T>
inline T SGL_ABS(T a)
{
   return (a >= 0 ? a : -a);
}

#ifndef SGL_SWAP
# define SGL_SWAP(a, b, temp) ((temp)=(a), (a)=(b), (b)=(temp))
#endif

template <typename T> bool Matrix4x4<T>::Invert4x4()
{
   Matrix4x4& self = *this;

   unsigned int indxc[4], indxr[4], ipiv[4];
   unsigned int i,j,k,l,ll;
   unsigned int icol = 0;
   unsigned int irow = 0;
   T temp, pivinv, dum, big;

   for (j = 0; j < 4; ++j)
   {
      ipiv[j] = 0;
   }

   for (i = 0; i < 4; ++i)
   {
      big = T(0);
      for (j = 0; j < 4; ++j)
      {
         if (ipiv[j] != 1)
         {
            for (k = 0; k < 4; ++k)
            {
               if (ipiv[k] == 0)
               {
                  if (SGL_ABS(self(j, k)) >= big)
                  {
                     big = SGL_ABS(self(j, k));
                     irow = j;
                     icol = k;
                  }
               }
               else if (ipiv[k] > 1)
               {
                  return false;
               }
            }
            ++(ipiv[icol]);
            if (irow != icol)
            {
               for (l = 0; l < 4; ++l)
               {
                  SGL_SWAP(
                     self(irow, l),
                     self(icol, l),
                     temp);
               }
            }

            indxr[i] = irow;
            indxc[i] = icol;
            if (self(icol, icol) == 0)
            {
               return false;
            }

            pivinv = T(1) / self(icol, icol);
            self(icol, icol) = 1;
            for (l = 0; l < 4; ++l) { self(icol, l) *= pivinv; }
            for (ll = 0; ll < 4; ++ll)
            {
               if (ll != icol)
               {
                  dum = self(ll, icol);
                  self(ll, icol) = 0;
                  for (l = 0; l < 4; ++l) { self(ll, l) -= self(icol, l)*dum; }
               }
            }
         }
      }
   }
   for (int lx = 4; lx > 0; --lx)
   {
      if (indxr[lx-1] != indxc[lx-1])
      {
         for (k = 0; k < 4; ++k)
         {
            SGL_SWAP(
               self(k, indxr[lx-1]),
               self(k, indxc[lx-1]),
               temp);
         }
      }
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_MATRIX__HPP
