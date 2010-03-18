#ifndef REACT3D_MATRIX__H
#define REACT3D_MATRIX__H

////////////////////////////////////////////////////////////////////////////////

#include <react3d/core/LibraryExport.h>

#include <react3d/core/math/Vec.h>

////////////////////////////////////////////////////////////////////////////////

// row (M) x column (N)
template <unsigned int M, unsigned int N, typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Matrix
{
public:
	Matrix() {}
	Matrix(const Matrix &matrix)
	{
		for (int m = 0; m < M; ++m)
		{
			for (int n = 0; n < N; ++n)
			{
				(*this)(m, n) = matrix(m, n);
			}
		}
	}

	T &operator()(int m, int n)
	{
		return data[m*M + n];
	}
	const T &operator()(int m, int n) const
	{
		return data[m*M + n];
	}

	const Vec<N, T> Row(int m) const
	{
		Vec<N, T> out;
		for (unsigned int n = 0; n < N; ++n)
			out[n] = (*this)(m, n);
		return out;
	}
	const Vec<M, T> Column(int n) const
	{
		Vec<M, T> out;
		for (unsigned int m = 0; m < M; ++m)
			out[m] = (*this)(m, n);
		return out;
	}

	// unary operators
	Matrix operator-() const
	{
		Matrix out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) = -(*this)[m, n];
			}
		}
		return out;
	}
	Matrix operator+() const
	{
		return (*this);
	}
	Matrix operator~() const // transpose
	{
		Matrix<N, M, T> out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(n, m) = (*this)[m, n];
			}
		}
		return out;
	}

	// binary operators
	Matrix operator+(const Matrix &matrix) const
	{
		Matrix out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) = (*this)(m, n) + matrix[m, n];
			}
		}
		return out;
	}

	Matrix operator-(const Matrix &matrix) const
	{
		Matrix out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) = (*this)(m, n) - matrix[m, n];
			}
		}
		return out;
	}

	template <unsigned int P> Matrix<M, P, T> operator*(const Matrix<N, P, T> &matrix) const
	{
		Matrix out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int p = 0; p < P; ++p)
			{
				out(m, p) = (*this).Row(m) * matrix.Column(p);
			}
		}
		return out;
	}

   Vec<M, T> operator*(const Vec<N, T> &vec) const
   {
      Vec<M, T> out;
      for (unsigned int m = 0; m < M; ++m)
      {
         out[m] = (*this).Row(m) * vec;
      }
      return out;
   }

	Matrix operator*(const T &t) const
	{
		Matrix out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) = (*this)(m, n) * t;
			}
		}
		return out;
	}

	Matrix operator/(const T &t) const
	{
		Matrix out;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) = (*this)(m, n) / t;
			}
		}
		return out;
	}

	Matrix operator+=(const Matrix &matrix)
	{
		Matrix &out = *this;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) += matrix[m, n];
			}
		}
		return out;
	}

	Matrix operator-=(const Matrix &matrix)
	{
		Matrix &out = *this;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) -= matrix[m, n];
			}
		}
		return out;
	}

   Matrix operator*=(const Matrix &matrix)
   {
      Matrix &out = *this;
      out = out * matrix;
      return out;
   }

	Matrix operator*=(const T &t)
	{
		Matrix &out = *this;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) *= t;
			}
		}
		return out;
	}

	Matrix operator/=(const T &t)
	{
		Matrix &out = *this;
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) /= t;
			}
		}
		return out;
	}

	// comparisons
	bool operator==(const Matrix &matrix) const
	{
		for (unsigned int m = 0; m < M; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				if ((*this)(m, n) != matrix(m, n))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Matrix &matrix) const
	{
		return !((*this) == matrix);
	}

private:
	T data[M*N];
};

template <unsigned int M, unsigned int N, typename T> inline Matrix<M, N, T> operator*(T t, const Matrix<M, N, T> &matrix)
{
	return matrix * t;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int N, typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ SquareMatrix : public Matrix<N, N, T>
{
public:
	SquareMatrix() {}
	SquareMatrix(const Matrix<N, N, T> &matrix)
	{
		for (unsigned int m = 0; m < N; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				(*this)(m, n) = matrix[m, n];
			}
		}
	}

	// unary operators
	T operator!() const // determinant
	{
		// Laplace's formula (note: inefficient for large n -- O(n!))
		T out = 0;
		// arbitrarily choose the first row to iterate along
		const unsigned int m = 0;
		for (unsigned int n = 0; n < N; ++n)
		{
			out += (*this)(m, n) * Cofactor(m, n);
		}
		return out;
	}

	// other matrices operations
	SquareMatrix<N-1, T> SubMatrix(unsigned int m, unsigned int n) const
	{
		// the (n - 1)(n - 1) matrix formed by
		//   removing the mth row and nth column
		SquareMatrix<N-1, T> out;
		for (unsigned int i = 0; i < N-1; ++i)
		{
			for (unsigned int j = 0; j < N-1; ++j)
			{
				out(i, j) = (*this)
					(i >= m ? i+1 : i,
					 j >= n ? j+1 : j);
			}
		}
		return out;
	}
	T Minor(unsigned int m, unsigned int n) const
	{
		// the determinant of the sub-matrix
		T out;
		out = !SubMatrix(m, n);
		return out;
	}
	T Cofactor(unsigned int m, unsigned int n) const
	{
		// (-1)^(m+n) times Minor(m, n)
		const bool even = ((m + n) % 2) == 0;
		T out = even ? T(1) : T(-1);
		out *= Minor(m, n);
		return out;
	}
	SquareMatrix CofactorMatrix() const
	{
		SquareMatrix out;
		for (unsigned int m = 0; m < N; ++m)
		{
			for (unsigned int n = 0; n < N; ++n)
			{
				out(m, n) = Cofactor(m, n);
			}
		}
		return out;
	}
	SquareMatrix Adjugate() const
	{
		SquareMatrix out;
		out = ~CofactorMatrix();
		return out;
	}
};

/* compile error -- todo fixme
// template specialization
template <typename T> T SquareMatrix<1, T>::operator!() const
{
	return (*this)(0, 0);
}
//*/

////////////////////////////////////////////////////////////////////////////////

template <typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Matrix2x2 : public SquareMatrix<2, T>
{
public:
	Matrix2x2() {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Matrix3x3 : public SquareMatrix<3, T>
{
public:
	Matrix3x3() {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Matrix4x4 : public SquareMatrix<4, T>
{
public:
	Matrix4x4() {}
   Matrix4x4(const Matrix<4, 4, T>& matrix)
   {
      for (unsigned int m = 0; m < 4; ++m)
      {
         for (unsigned int n = 0; n < 4; ++n)
         {
            (*this)(m, n) = matrix(m, n);
         }
      }
   }
   Matrix4x4(T a00, T a01, T a02, T a03,
             T a10, T a11, T a12, T a13,
             T a20, T a21, T a22, T a23,
             T a30, T a31, T a32, T a33)
   {
      (*this)(0, 0) = a00; (*this)(0, 1) = a01; (*this)(0, 2) = a02; (*this)(0, 3) = a03;
      (*this)(1, 0) = a10; (*this)(1, 1) = a11; (*this)(1, 2) = a12; (*this)(1, 3) = a13;
      (*this)(2, 0) = a20; (*this)(2, 1) = a21; (*this)(2, 2) = a22; (*this)(2, 3) = a23;
      (*this)(3, 0) = a30; (*this)(3, 1) = a31; (*this)(3, 2) = a32; (*this)(3, 3) = a33;
   }

   static Matrix4x4 Zero()
   {
      return Matrix4x4(0, 0, 0, 0,
                       0, 0, 0, 0,
                       0, 0, 0, 0,
                       0, 0, 0, 0);
   }
   static Matrix4x4 Identity()
   {
      return Matrix4x4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);
   }

   void SetTranslation(const Vec3<T>& translation)
   {
      (*this)(3, 0) = translation[X];
      (*this)(3, 1) = translation[Y];
      (*this)(3, 2) = translation[Z];
   }

   Vec3<T> GetTranslation() const
   {
      Vec3<T> translation;
      translation[X] = (*this)(3, 0);
      translation[Y] = (*this)(3, 1);
      translation[Z] = (*this)(3, 2);
      return translation;
   }

   void SetRotate(const Vec4<T>& quat);
   /** Get the matrix rotation as a Quat. Note that this function
    * assumes a non-scaled matrix and will return incorrect results
    * for scaled matrices. Consider decompose() instead.
    */
   Vec4<T> GetRotate() const;

   bool Invert();
   Matrix4x4 Inverse() const;

protected:
   bool Invert4x3();
   bool Invert4x4();
};

////////////////////////////////////////////////////////////////////////////////

typedef Matrix2x2<float> Matrix2x2f;
typedef Matrix3x3<float> Matrix3x3f;
typedef Matrix4x4<float> Matrix4x4f;

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_MATRIX__H
