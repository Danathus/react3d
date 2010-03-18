#ifndef REACT3D_VEC__H
#define REACT3D_VEC__H

////////////////////////////////////////////////////////////////////////////////

#include <math.h>

#include <react3d/core/LibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

#define CLAMP(_min, _var, _max) if (_var < _min) _var = _min; if (_var > _max) _var = _max;
#define WRAP(_min, _var, _max) if (_var < _min) _var += (_max - _min); if (_var > _max) _var -= (_max - _min);

enum
{
	X = 0,
	Y,
	Z,
	W
};

enum
{
	R = 0,
	G,
	B,
	A
};

enum
{
	U = 0,
	V
};

template <unsigned int N, typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Vec
{
public:
	Vec() {}
	Vec(const Vec &v)
	{
		for (unsigned int i = 0; i < N; ++i)
			(*this)[i] = v[i];
	}

	T &operator[](int i)
	{
		return data[i];
	}
	const T &operator[](int i) const
	{
		return data[i];
	}

	// unary operators
	Vec operator-() const
	{
		Vec out;
		for (unsigned int i = 0; i < N; ++i)
			out[i] = -(*this)[i];
		return out;
	}
	Vec operator+() const
	{
		return (*this);
	}
	T operator!() const
	{
		T out = sqrtf((*this) * (*this));
		return out;
	}
	Vec operator~() const
	{
		Vec out = (*this) / !(*this);
		return out;
	}

	// binary operators
	Vec operator+(const Vec &v) const
	{
		Vec out;
		for (unsigned int i = 0; i < N; ++i)
			out[i] = (*this)[i] + v[i];
		return out;
	}

	Vec operator-(const Vec &v) const
	{
		Vec out;
		for (unsigned int i = 0; i < N; ++i)
			out[i] = (*this)[i] - v[i];
		return out;
	}

	T operator*(const Vec &v) const
	{
		T out = 0.0f;
		for (unsigned int i = 0; i < N; ++i)
			out += (*this)[i] * v[i];
		return out;
	}

	Vec operator*(const T &t) const
	{
		Vec out;
		for (unsigned int i = 0; i < N; ++i)
			out[i] = (*this)[i] * t;
		return out;
	}

	Vec operator/(const T &t) const
	{
		Vec out;
		for (unsigned int i = 0; i < N; ++i)
			out[i] = (*this)[i] / t;
		return out;
	}

	Vec operator+=(const Vec &v)
	{
		Vec &out = *this;
		for (unsigned int i = 0; i < N; ++i)
			out[i] += v[i];
		return out;
	}

	Vec operator-=(const Vec &v)
	{
		Vec &out = *this;
		for (unsigned int i = 0; i < N; ++i)
			out[i] -= v[i];
		return out;
	}

	Vec operator*=(const T &t)
	{
		Vec &out = *this;
		for (unsigned int i = 0; i < N; ++i)
			out[i] *= t;
		return out;
	}

	Vec operator/=(const T &t)
	{
		Vec &out = *this;
		for (unsigned int i = 0; i < N; ++i)
			out[i] /= t;
		return out;
	}

	// comparisons
	bool operator==(const Vec &v) const
	{
		for (unsigned int i = 0; i < N; ++i)
			if (data[i] != v[i])
				return false;
		return true;
	}

	bool operator!=(const Vec &v) const
	{
		return !((*this) == v);
	}

   // constants
   static Vec Zero()
   {
      Vec zero;
      for (unsigned int i = 0; i < N; ++i)
         zero[i] = 0;
      return zero;
   }

private:
	T data[N];
};

template <unsigned int N, typename T> inline Vec<N, T> operator*(const T &t, const Vec<N, T> &v)
{
	return v * t;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Vec2 : public Vec<2, T>
{
public:
	Vec2() {}
	Vec2(const Vec<2, T> &v)
	{
		for (int i = 0; i < 2; ++i)
			(*this)[i] = v[i];
	}
	Vec2(const T &x, const T &y)
	{
		(*this)[X] = x;
		(*this)[Y] = y;
	}

	// complex product
	Vec2 operator^(const Vec2 &v) const
	{
		const Vec2 &in = *this;

		Vec2 out(in[X]*v[X] - in[Y]*v[Y], in[X]*v[Y] + in[Y]*v[X]);

		return out;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Vec3 : public Vec<3, T>
{
public:
	Vec3() {}
	Vec3(const Vec<3, T> &v)
	{
		for (int i = 0; i < 3; ++i)
			(*this)[i] = v[i];
	}
	Vec3(const T &x, const T &y, const T &z)
	{
		(*this)[X] = x;
		(*this)[Y] = y;
		(*this)[Z] = z;
	}

	Vec3 RotX(const T &angX) const
	{
		const Vec3 &in = *this;

		T sinX = sinf(angX * 3.1415926f / 180.0f);
		T cosX = cosf(angX * 3.1415926f / 180.0f);

		const Vec3 out = Vec3(
			in[X],
			(cosX * in[Y]) + (-sinX * in[Z]),
			(sinX * in[Y]) + ( cosX * in[Z]) );

		return out;
	}

	Vec3 RotZ(const T &angZ) const
	{
		const Vec3 &in = *this;

		float sinZ = sinf(angZ * 3.1415926f / 180.0f);
		float cosZ = cosf(angZ * 3.1415926f / 180.0f);

		const Vec3 out = Vec3(
			(cosZ * in[X]) + (-sinZ * in[Y]),
			(sinZ * in[X]) + ( cosZ * in[Y]),
			in[Z]);

		return out;
	}

	// cross product
	Vec3 operator^(const Vec3 &v) const
	{
		const Vec3 &in = *this;

		Vec3 out(in[Y]*v[Z] - in[Z]*v[Y], in[Z]*v[X] - in[X]*v[Z], in[X]*v[Y] - in[Y]*v[X]);

		return out;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T> class /*REACT3D_CORE_LIBRARY_EXPORT*/ Vec4 : public Vec<4, T>
{
public:
	Vec4() {}
	Vec4(const Vec<4, T> &v)
	{
		for (int i = 0; i < 4; ++i)
			(*this)[i] = v[i];
	}
	Vec4(const T &x, const T &y, const T &z, const T &w)
	{
		(*this)[X] = x;
		(*this)[Y] = y;
		(*this)[Z] = z;
		(*this)[W] = w;
	}

	// quaternion product
	Vec4 operator^(const Vec4 &v) const
	{
		const Vec4 &in = *this;

		/* (a + bi + cj + dk) * (e + fi + gj + hk)
			=
			a(e + fi + gj + hk)  +
			b(e + fi + gj + hk)i +
			c(e + fi + gj + hk)j +
			d(e + fi + gj + hk)k
			=
			(ae - bf - cg - dh) +
			(af + be - ch + dg)i +
			(ag + bh + ce - df)j +
			(ah - bg + cf + de)k
		//*/

		// using the fourth component for the scalar
		const Vec4 out(
			in[W]*v[W] - in[X]*v[X] - in[Y]*v[Y] - in[Z]*v[Z],
			in[W]*v[X] + in[X]*v[W] - in[Y]*v[Z] + in[Z]*v[Y],
			in[W]*v[Y] + in[X]*v[Z] + in[Y]*v[W] - in[Z]*v[X],
			in[W]*v[Z] - in[X]*v[Y] + in[Y]*v[X] + in[Z]*v[W]);

		return out;
	}
};

////////////////////////////////////////////////////////////////////////////////

typedef Vec2<int> Vec2i;

typedef Vec2<float> Vec2f;
typedef Vec3<float> Vec3f;
typedef Vec4<float> Vec4f;

typedef Vec4f Color4f;
typedef Vec4f Quaternionf;

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_VEC__H
