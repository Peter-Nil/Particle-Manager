#include <assert.h>
#include "Vector4.h"

using namespace pc;

Vector4::Vector4()
{
	mX = 0;
	mY = 0;
	mZ = 0;
	mW = 0;
}

Vector4::Vector4(float x, float y)
{
	mX = x;
	mY = y;
	mZ = 0;
	mW = 0;
}

Vector4::Vector4(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
	mW = 0;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	mX = x;
	mY = y;
	mZ = z;
	mW = w;
}

Vector4::Vector4(const Vector4& v)
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
	mW = v.mW;
}


float Vector4::length() const
{
	return sqrtf(mX * mX + mY * mY + mZ * mZ + mW * mW);
}

float Vector4::sqrLength() const
{
	return mX * mX + mY * mY + mZ * mZ + mW * mW;
}

void Vector4::normalize()
{
	float l = length();
	if(l > 0.000001) 
	{
		mX *= 1.0f / l;
		mY *= 1.0f / l;
		mZ *= 1.0f / l;
		mW *= 1.0f / l;
	}
}

Vector4 Vector4::componentProduct(const Vector4& v) const
{
	return Vector4(mX * v.mX, mY * v.mY, mZ * v.mZ, mW * v.mW);
}


Vector4 Vector4::getNormal() const
{
	Vector4 res = Vector4(*this);
	res.normalize();
	return res;
}


Vector4 Vector4::operator+(float s) const
{
	return Vector4(mX + s, mY + s, mZ + s, mW + s);
}

Vector4 Vector4::operator+(const Vector4& v2) const
{
	return Vector4(mX + v2.mX, mY + v2.mY, mZ + v2.mZ, mW + v2.mW);
}

const Vector4& Vector4::operator+=(float s)
{
	mX += s;
	mY += s;
	mZ += s;
	mW += s;
	return *this;
}

const Vector4& Vector4::operator+=(const Vector4& v2)
{
	mX += v2.mX;
	mY += v2.mY;
	mZ += v2.mZ;
	mW += v2.mW;
	return *this;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-mX, -mY, -mZ, -mW);
}

Vector4 Vector4::operator-(float s) const
{
	return Vector4(mX - s, mY - s, mZ - s, mW - s);
}

Vector4 Vector4::operator-(const Vector4& v2) const
{
	return Vector4(mX - v2.mX, mY - v2.mY, mZ - v2.mZ, mW - v2.mW);
}

const Vector4& Vector4::operator-=(float s)
{
	mX -= s;
	mY -= s;
	mZ -= s;
	mW -= s;
	return *this;
}

const Vector4& Vector4::operator-=(const Vector4& v2)
{
	mX -= v2.mX;
	mY -= v2.mY;
	mZ -= v2.mZ;
	mW -= v2.mW;
	return *this;
}

Vector4 Vector4::operator*(float s) const
{
	return Vector4(mX * s, mY * s, mZ * s, mW * s);
}

float Vector4::operator*(const Vector4& v2) const
{
	return mX * v2.mX + mY * v2.mY + mZ * v2.mZ + mW * v2.mW;
}

const Vector4& Vector4::operator*=(float s)
{
	mX *= s;
	mY *= s;
	mZ *= s;
	mW *= s;
	return *this;
}

float Vector4::operator[](unsigned i) const
{
	assert((i < 4) || !"Vector4::(i) i must be in range [0..3]");
	return *(&mX + i);
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics