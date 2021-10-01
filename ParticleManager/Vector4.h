#pragma once
#include <cmath>

namespace pc 
{
	// 4d vector.
	class Vector4
	{
	public:
		float mX, mY, mZ, mW;

		Vector4();
		Vector4(float x, float y);
		Vector4(float x, float y, float z);
		Vector4(float x, float y, float z, float w);
		Vector4(const Vector4& v);

		float length() const;
		// Gets the squared length.
		float sqrLength() const;
		void normalize();
		// Calculates and returns a componentwise product of this vector with the given vector.
		Vector4 componentProduct(const Vector4& v) const;
		
		Vector4 getNormal() const;

		Vector4 operator+(float s) const;
		Vector4 operator+(const Vector4& v2) const;
		const Vector4& operator+= (float s);
		const Vector4& operator+= (const Vector4& v2);
		Vector4 operator-() const;
		Vector4 operator-(float s) const;
		Vector4 operator-(const Vector4& v2) const;
		const Vector4& operator-=(float s);
		const Vector4& operator-= (const Vector4& v2);
		Vector4 operator*(float s) const;
		float operator*(const Vector4& v2) const;
		const Vector4& operator*=(float s);
		float operator[](unsigned i) const;
	};
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics