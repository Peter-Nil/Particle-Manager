#pragma once
#include "Point.h"
#include <cmath>

namespace pc
{
	// 3d vector.
	class Vector
	{
	public:
		float mX, mY, mZ;
		const static Vector UP;
		const static Vector RIGHT;
		const static Vector OUT_OF_SCREEN;

		// Zero vector.
		Vector();
		Vector(float x, float y);
		Vector(float x, float y, float z);
		Vector(const Point& p);
		Vector(const Vector& v);
		
		float length() const;
		// Gives the squared magnitude/lenght of this vector.
		float sqrLength() const;
		void normalize();
		// Calculates and returns a componentwise product of this vector with the given vector.
		Vector componentProduct(const Vector& vector) const;

		Point getPoint() const;
		Vector getNormal() const;

		Vector operator+(float s) const;
		Vector operator+(const Vector& v2) const;
		const Vector& operator+=(float s);
		const Vector& operator+=(const Vector& v2);
		Vector operator-() const;
		Vector operator-(float s) const;
		Vector operator-(const Vector& v2) const;
		const Vector& operator-=(float s);
		const Vector& operator-=(const Vector& v2);
		Vector operator*(float s) const;
		float operator*(const Vector& v2) const;
		const Vector& operator*=(float s);
		Vector operator%(const Vector& v2) const;
		float operator[](unsigned i) const;
	};
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics