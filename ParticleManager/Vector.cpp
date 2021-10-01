#include <assert.h>
#include "Vector.h"

using namespace pc;

const Vector Vector::UP = Vector(0, 1, 0);
const Vector Vector::RIGHT = Vector(1, 0, 0);
const Vector Vector::OUT_OF_SCREEN = Vector(0, 0, 1);

Vector::Vector()
{
	mX = 0;
	mY = 0;
	mZ = 0;
}

Vector::Vector(float x, float y)
{
	mX = x;
	mY = y;
	mZ = 0;
}

Vector::Vector(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

Vector::Vector(const Point& p)
{
	mX = p.mX;
	mY = p.mY;
	mZ = p.mZ;
}

Vector::Vector(const Vector& v)
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
}



float Vector::length() const
{
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

float Vector::sqrLength() const
{
	return mX * mX + mY * mY + mZ * mZ;
}

void Vector::normalize()
{
	float l = length();
	if (l > 0.000001)
	{
		mX *= 1.0f / l;
		mY *= 1.0f / l;
		mZ *= 1.0f / l;
	}
}

Vector Vector::componentProduct(const Vector& v) const
{
	return Vector(mX * v.mX, mY * v.mY, mZ * v.mZ);
}


Point Vector::getPoint() const
{
	return Point(mX, mY, mZ);
}

Vector Vector::getNormal() const
{
	Vector res = Vector(*this);
	res.normalize();
	return res;
}


Vector Vector::operator+(float s) const
{
	return Vector(mX + s, mY + s, mZ + s);
}

Vector Vector::operator+(const Vector& v2) const
{
	return Vector(mX + v2.mX, mY + v2.mY, mZ + v2.mZ);
}

const Vector& Vector::operator+=(float s)
{
	mX += s;
	mY += s;
	mZ += s;
	return *this;
}

const Vector& Vector::operator+=(const Vector& v2)
{
	mX += v2.mX;
	mY += v2.mY;
	mZ += v2.mZ;
	return *this;
}

Vector Vector::operator-() const
{
	return Vector(-mX, -mY, -mZ);
}

Vector Vector::operator-(float s) const
{
	return Vector(mX - s, mY - s, mZ - s);
}

Vector Vector::operator-(const Vector& v2) const
{
	return Vector(mX - v2.mX, mY - v2.mY, mZ - v2.mZ);
}

const Vector& Vector::operator-=(float s)
{
	mX -= s;
	mY -= s;
	mZ -= s;
	return *this;
}

const Vector& Vector::operator-=(const Vector& v2)
{
	mX -= v2.mX;
	mY -= v2.mY;
	mZ -= v2.mZ;
	return *this;
}

Vector Vector::operator*(float s) const
{
	return Vector(mX * s, mY * s, mZ * s);
}

float Vector::operator*(const Vector& v2) const
{
	return mX * v2.mX + mY * v2.mY + mZ * v2.mZ;
}

const Vector& Vector::operator*=(float s)
{
	mX *= s;
	mY *= s;
	mZ *= s;
	return *this;
}

Vector Vector::operator%(const Vector& v2) const
{
	return Vector(
		mY * v2.mZ - mZ * v2.mY,
		mZ * v2.mX - mX * v2.mZ,
		mX * v2.mY - mY * v2.mX);
}

float Vector::operator[](unsigned i) const
{
	assert((i < 3) || !"Vector::(i) i must be in range [0..2]");
	return *(&mX + i);
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics