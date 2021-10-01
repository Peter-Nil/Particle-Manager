#pragma once
#include <cmath>
#include "Vector.h"
#include "Matrix.h"
#include "Constants.h"

namespace pc
{
	// Mostly used to represent orientation.
	class Quaternion
	{
	public:
		float mS, mX, mY, mZ;

		// Creates a quaternion which doesnt rotate a vector. That means s = 1.
		Quaternion();
		Quaternion(float s, float x, float y, float z);
		// Initiates the quaternion to represent a rotation (degrees) around the direction. See 9.2.4 in Ians book.
		Quaternion(int degrees, const Vector& rotDir);
		Quaternion(const Vector& v);
		Quaternion(const Quaternion& q);

		// Gives the rotation matrix from this unit quaternion.
		Matrix toMatrix() const;
		// Transforms an unit quaternion and a position to a transformation 4x4 matrix.
		Matrix4 toMatrix4(const Vector& position) const;
		// Gives the length of the quaternion.
		float length() const;
		// Gives a unit quaternion as long as we dont have a zero quaternion.
		void normalize();
		// Gives the conjugate/inverse of the quaternion.
		Quaternion conjugate() const;
		// Gives a vector rotated by the angle and direction given by this quaternion as long the quaternion has the length of 1.
		Vector rotate(const Vector& v) const;
		// Inversefunction of rotate().
		Vector rotateBack(const Vector& v) const;

		// Gives the rotational axis.
		Vector getRotationAxis();
		// Gives the angle (in degrees) used to rotate around the rotational axis.
		float getRotationInDegrees();

		// Does componentwise addition.
		Quaternion operator+(const Quaternion& q2) const;
		const Quaternion& operator+=(const Quaternion& q2);
		// Does componentwise subtraction.
		Quaternion operator-(const Quaternion& q2) const;
		const Quaternion& operator-= (const Quaternion& q2);
		// Gives a quaternion scaled with f.
		Quaternion operator*(float f) const;
		// Multiplicationsoperator. Works like q1*q2 = (a1+b1i+c1j+d1k)(a2+b2i+c2j+d2k) = etc.
		Quaternion operator*(const Quaternion& q2) const;
		// Scales the quaternion with f.
		const Quaternion& operator*=(float f);
		const Quaternion& operator*=(const Quaternion& q2);
	};
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics