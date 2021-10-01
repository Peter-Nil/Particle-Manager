#include "Quaternion.h"

using namespace pc;

Quaternion::Quaternion()
{
	mS = 1;
	mX = mY = mZ = 0;
}

Quaternion::Quaternion(float s, float x, float y, float z)
{
	mS = s;
	mX = x;
	mY = y;
	mZ = z;
}

Quaternion::Quaternion(int degrees, const Vector& rotDir)
{
	Vector dir = rotDir;
	dir.normalize(); // To be sure we are using a normal.
	float alpha = 3.1415926f / 180.0f * degrees; // Rotation angle in radians.
	dir *= sin(alpha * 0.5f); // Direction scales with sinus for half angle.
	mS = cos(alpha * 0.5f);
	mX = dir.mX;
	mY = dir.mY;
	mZ = dir.mZ; // The value of the scalar is cosinus of the half angle.
}

Quaternion::Quaternion(const Vector& v)
{
	mS = 0;
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
}

Quaternion::Quaternion(const Quaternion& q)
{
	mS = q.mS;
	mX = q.mX;
	mY = q.mY;
	mZ = q.mZ;
}


Matrix Quaternion::toMatrix() const
{
	Matrix ret;
	ret.m11 = 1 -	2 * mY * mY - 2 * mZ * mZ;
	ret.m21 =		2 * mX * mY + 2 * mS * mZ;
	ret.m31 =		2 * mX * mZ - 2 * mS * mY;

	ret.m12 =		2 * mX * mY - 2 * mS * mZ;
	ret.m22 = 1 -	2 * mX * mX - 2 * mZ * mZ;
	ret.m32 =		2 * mY * mZ + 2 * mS * mX;

	ret.m13 =		2 * mX * mZ + 2 * mS * mY;
	ret.m23 =		2 * mY * mZ - 2 * mS * mX;
	ret.m33 = 1 -	2 * mX * mX - 2 * mY * mY;
	return ret;
}

Matrix4 Quaternion::toMatrix4(const Vector& position) const
{
	Matrix4 ret;
	ret.m11 = 1 -	2 * mY * mY - 2 * mZ * mZ;
	ret.m12 =		2 * mX * mY - 2 * mS * mZ;
	ret.m13 =		2 * mX * mZ + 2 * mS * mY;
	ret.m14 = position.mX;

	ret.m21 =		2 * mX * mY + 2 * mS * mZ;
	ret.m22 = 1 -	2 * mX * mX - 2 * mZ * mZ;
	ret.m23 =		2 * mY * mZ - 2 * mS * mX;
	ret.m24 = position.mY;

	ret.m31 =		2 * mX * mZ - 2 * mS * mY;
	ret.m32 =		2 * mY * mZ + 2 * mS * mX;
	ret.m33 = 1 -	2 * mX * mX - 2 * mY * mY;
	ret.m34 = position.mZ;

	ret.m41 = ret.m42 = ret.m43 = 0;
	ret.m44 = 1;
	return ret;
}

float Quaternion::length() const
{
	return sqrtf(mS * mS + mX * mX + mY * mY + mZ * mZ);
}

void Quaternion::normalize()
{
	float l = length();
	if (l > SMALLTHRESHOLD)
	{
		mS *= 1 / l;
		mX *= 1 / l;
		mY *= 1 / l;
		mZ *= 1 / l;
	}
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(mS, -mX, -mY, -mZ);
}

Vector Quaternion::rotate(const Vector& v) const
{
	Quaternion res = (*this) * Quaternion(v) * (*this).conjugate();
	return Vector(res.mX, res.mY, res.mZ);
}

Vector Quaternion::rotateBack(const Vector& v) const
{
	Quaternion res = (*this).conjugate() * Quaternion(v) * (*this);
	return Vector(res.mX, res.mY, res.mZ);
}


Vector Quaternion::getRotationAxis()
{
	Vector res = Vector(mX, mY, mZ);
	res.normalize();
	return res;
}

float Quaternion::getRotationInDegrees()
{
	float rad = acos(mS) * 2;
	return 180 * rad / 3.1415926f;
}


Quaternion Quaternion::operator+(const Quaternion& q2) const
{
	return Quaternion(mS + q2.mS, mX + q2.mX, mY + q2.mY, mZ + q2.mZ);
}

const Quaternion& Quaternion::operator+=(const Quaternion& q2)
{
	mS += q2.mS;
	mX += q2.mX;
	mY += q2.mY;
	mZ += q2.mZ;
	return *this;
}

Quaternion Quaternion::operator-(const Quaternion& q2) const
{
	return Quaternion(mS - q2.mS, mX - q2.mX, mY - q2.mY, mZ - q2.mZ);
}

const Quaternion& Quaternion::operator-=(const Quaternion& q2)
{
	mS -= q2.mS;
	mX -= q2.mX;
	mY -= q2.mY;
	mZ -= q2.mZ;
	return *this;
}

Quaternion Quaternion::operator*(float f) const
{
	return Quaternion(mS * f, mX * f, mY * f, mZ * f);
}

Quaternion Quaternion::operator*(const Quaternion& q2) const
{
	float s = mS * q2.mS - mX * q2.mX - mY * q2.mY - mZ * q2.mZ;
	float x = mS * q2.mX + mX * q2.mS + mY * q2.mZ - mZ * q2.mY;
	float y = mS * q2.mY - mX * q2.mZ + mY * q2.mS + mZ * q2.mX;
	float z = mS * q2.mZ + mX * q2.mY - mY * q2.mX + mZ * q2.mS;
	return Quaternion(s, x, y, z);
}

const Quaternion& Quaternion::operator*=(float f)
{
	mS *= f;
	mX *= f;
	mY *= f;
	mZ *= f;
	return *this;
}

const Quaternion& Quaternion::operator*=(const Quaternion& q2)
{
	return (*this) = (*this) * q2;
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics