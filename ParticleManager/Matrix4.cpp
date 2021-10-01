#include <assert.h>
#include "Matrix4.h"

using namespace pc;

Matrix4::Matrix4()
{
	m12 = m13 = m14 = 
	m21 = m23 = m24 =
	m31 = m32 = m34	=
	m41 = m42 = m43	= 0;

	m11 = m22 = m33 = m44 = 1;
}

Matrix4::Matrix4(
	float p11, float p12, float p13, float p14,
	float p21, float p22, float p23, float p24,
	float p31, float p32, float p33, float p34,
	float p41, float p42, float p43, float p44)
{
	m11 = p11; m12 = p12; m13 = p13; m14 = p14;
	m21 = p21; m22 = p22; m23 = p23; m24 = p24;
	m31 = p31; m32 = p32; m33 = p33; m34 = p34;
	m41 = p41; m42 = p42; m43 = p43; m44 = p44;
}

Matrix4::Matrix4(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4)
{
	m11 = col1.mX; m12 = col2.mX; m13 = col3.mX; m14 = col4.mX;
	m21 = col1.mY; m22 = col2.mY; m23 = col3.mY; m24 = col4.mY;
	m31 = col1.mZ; m32 = col2.mZ; m33 = col3.mZ; m34 = col4.mZ;
	m41 = col1.mW; m42 = col2.mW; m43 = col3.mW; m44 = col4.mW;
}

Matrix4::Matrix4(const Matrix4& m)
{
	m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
	m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
	m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
	m41 = m.m41; m42 = m.m42; m43 = m.m43; m44 = m.m44;
}


void Matrix4::transpose()
{
	Matrix4 tmp = (*this);
	m12 = tmp.m21;
	m13 = tmp.m31;
	m14 = tmp.m41;

	m21 = tmp.m12;
	m23 = tmp.m32;
	m24 = tmp.m42;

	m31 = tmp.m13;
	m32 = tmp.m23;
	m34 = tmp.m43;

	m41 = tmp.m14;
	m42 = tmp.m24;
	m43 = tmp.m34;
}

void Matrix4::invert()
{
	float det = determinant();
	// Makes sure the determinant is non-zero and thus makes the matrix invertable.
	if(det * det < 0.000001f) return; 

	float nm11 = (m22 * m33 * m44 - m22 * m34 * m43 - m32 * m23 * m44 + m32 * m24 * m43 + m42 * m23 * m34 - m42 * m24 * m33) / det;
	float nm21 =(-m21 * m33 * m44 + m21 * m34 * m43 + m31 * m23 * m44 - m31 * m24 * m43 - m41 * m23 * m34 + m41 * m24 * m33) / det;
	float nm31 = (m21 * m32 * m44 - m21 * m34 * m42 - m31 * m22 * m44 + m31 * m24 * m42 + m41 * m22 * m34 - m41 * m24 * m32) / det;
	float nm41 =(-m21 * m32 * m43 + m21 * m33 * m42 + m31 * m22 * m43 - m31 * m23 * m42 - m41 * m22 * m33 + m41 * m23 * m32) / det;

	float nm12 =(-m12 * m33 * m44 + m12 * m34 * m43 + m32 * m13 * m44 - m32 * m14 * m43 - m42 * m13 * m34 + m42 * m14 * m33) / det;
	float nm22 = (m11 * m33 * m44 - m11 * m34 * m43 - m31 * m13 * m44 + m31 * m14 * m43 + m41 * m13 * m34 - m41 * m14 * m33) / det;
	float nm32 =(-m11 * m32 * m44 + m11 * m34 * m42 + m31 * m12 * m44 - m31 * m14 * m42 - m41 * m12 * m34 + m41 * m14 * m32) / det;
	float nm42 = (m11 * m32 * m43 - m11 * m33 * m42 - m31 * m12 * m43 + m31 * m13 * m42 + m41 * m12 * m33 - m41 * m13 * m32) / det;

	float nm13 = (m12 * m23 * m44 - m12 * m24 * m43 - m22 * m13 * m44 + m22 * m14 * m43 + m42 * m13 * m24 - m42 * m14 * m23) / det;
	float nm23 =(-m11 * m23 * m44 + m11 * m24 * m43 + m21 * m13 * m44 - m21 * m14 * m43 - m41 * m13 * m24 + m41 * m14 * m23) / det;
	float nm33 = (m11 * m22 * m44 - m11 * m24 * m42 - m21 * m12 * m44 + m21 * m14 * m42 + m41 * m12 * m24 - m41 * m14 * m22) / det;
	float nm43 =(-m11 * m22 * m43 + m11 * m23 * m42 + m21 * m12 * m43 - m21 * m13 * m42 - m41 * m12 * m23 + m41 * m13 * m22) / det;

	float nm14 =(-m12 * m23 * m34 + m12 * m24 * m33 + m22 * m13 * m34 - m22 * m14 * m33 - m32 * m13 * m24 + m32 * m14 * m23) / det;
	float nm24 = (m11 * m23 * m34 - m11 * m24 * m33 - m21 * m13 * m34 + m21 * m14 * m33 + m31 * m13 * m24 - m31 * m14 * m23) / det;
	float nm34 =(-m11 * m22 * m34 + m11 * m24 * m32 + m21 * m12 * m34 - m21 * m14 * m32 - m31 * m12 * m24 + m31 * m14 * m22) / det;
	float nm44 = (m11 * m22 * m33 - m11 * m23 * m32 - m21 * m12 * m33 + m21 * m13 * m32 + m31 * m12 * m23 - m31 * m13 * m22) / det;		

	m11 = nm11; m12 = nm12; m13 = nm13; m14 = nm14;
	m21 = nm21; m22 = nm22; m23 = nm23; m24 = nm24;
	m31 = nm31; m32 = nm32; m33 = nm33; m34 = nm34;
	m41 = nm41; m42 = nm42; m43 = nm43; m44 = nm44;
}

float Matrix4::determinant() const
{
	return
		m14 * m23 * m32 * m41 - m13 * m24 * m32 * m41 - m14 * m22 * m33 * m41 + m12 * m24 * m33 * m41 +
		m13 * m22 * m34 * m41 - m12 * m23 * m34 * m41 - m14 * m23 * m31 * m42 + m13 * m24 * m31 * m42 +
		m14 * m21 * m33 * m42 - m11 * m24 * m33 * m42 - m13 * m21 * m34 * m42 + m11 * m23 * m34 * m42 +
		m14 * m22 * m31 * m43 - m12 * m24 * m31 * m43 - m14 * m21 * m32 * m43 + m11 * m24 * m32 * m43 +
		m12 * m21 * m34 * m43 - m11 * m22 * m34 * m43 - m13 * m22 * m31 * m44 + m12 * m23 * m31 * m44 +
		m13 * m21 * m32 * m44 - m11 * m23 * m32 * m44 - m12 * m21 * m33 * m44 + m11 * m22 * m33 * m44;
}

Vector Matrix4::transformInvRot(const Vector& vector) const
{
	Vector tmp = vector;
	tmp.mX -= m14;
	tmp.mY -= m24;
	tmp.mZ -= m34;
	return Vector(
		tmp.mX * m11 + tmp.mY * m21 + tmp.mZ * m31,
		tmp.mX * m12 + tmp.mY * m22 + tmp.mZ * m32,
		tmp.mX * m13 + tmp.mY * m23 + tmp.mZ * m33);
}


Vector Matrix4::getColumnVector(unsigned i) const
{
	assert((i < 4) || !"Vector::(i) i must be in range [0..3]");
	return Vector(*(&m11 + i), *(&m21 + i), *(&m31 + i));
}

Matrix4 Matrix4::getTranspose() const
{
	Matrix4 result = Matrix4(*this);
	result.transpose();
	return result;
}

Matrix4 Matrix4::getInverse() const
{
	Matrix4 result = Matrix4(*this);
	result.invert();
	return result;
}


Matrix4 Matrix4::operator+(const Matrix4& m2) const
{
	Matrix4 res = Matrix4(*this);
	res.m11 += m2.m11; res.m12 += m2.m12; res.m13 += m2.m13; res.m14 += m2.m14;
	res.m21 += m2.m21; res.m22 += m2.m22; res.m23 += m2.m23; res.m24 += m2.m24;
	res.m31 += m2.m31; res.m32 += m2.m32; res.m33 += m2.m33; res.m34 += m2.m34;
	res.m31 += m2.m41; res.m42 += m2.m42; res.m33 += m2.m43; res.m44 += m2.m44;
	return res;
}

const Matrix4& Matrix4::operator+=(const Matrix4& m2)
{
	m11 += m2.m11; m12 += m2.m12; m13 += m2.m13; m14 += m2.m14;
	m21 += m2.m21; m22 += m2.m22; m23 += m2.m23; m24 += m2.m24;
	m31 += m2.m31; m32 += m2.m32; m33 += m2.m33; m34 += m2.m34;
	m31 += m2.m41; m42 += m2.m42; m33 += m2.m43; m44 += m2.m44;
	return *this;
}

Matrix4 Matrix4::operator-(const Matrix4& m2) const
{
	Matrix4 res = Matrix4(*this);
	res.m11 -= m2.m11; res.m12 -= m2.m12; res.m13 -= m2.m13; res.m14 -= m2.m14;
	res.m21 -= m2.m21; res.m22 -= m2.m22; res.m23 -= m2.m23; res.m24 -= m2.m24;
	res.m31 -= m2.m31; res.m32 -= m2.m32; res.m33 -= m2.m33; res.m34 -= m2.m34;
	res.m31 -= m2.m41; res.m42 -= m2.m42; res.m33 -= m2.m43; res.m44 -= m2.m44;
	return res;
}

const Matrix4& Matrix4::operator-=(const Matrix4& m2)
{
	m11 -= m2.m11; m12 -= m2.m12; m13 -= m2.m13; m14 -= m2.m14;
	m21 -= m2.m21; m22 -= m2.m22; m23 -= m2.m23; m24 -= m2.m24;
	m31 -= m2.m31; m32 -= m2.m32; m33 -= m2.m33; m34 -= m2.m34;
	m31 -= m2.m41; m42 -= m2.m42; m33 -= m2.m43; m44 -= m2.m44;
	return *this;
}

Matrix4 Matrix4::operator*(float s) const
{
	Matrix4 res = Matrix4(*this);
	res.m11 *= s; res.m12 *= s; res.m13 *= s; res.m14 *= s;
	res.m21 *= s; res.m22 *= s; res.m23 *= s; res.m24 *= s;
	res.m31 *= s; res.m32 *= s; res.m33 *= s; res.m34 *= s;
	res.m31 *= s; res.m42 *= s; res.m33 *= s; res.m44 *= s;
	return res;
}

Vector Matrix4::operator*(const Vector& v) const
{
	return Vector(
		m11 * v.mX + m12 * v.mY + m13 * v.mZ + m14,
		m21 * v.mX + m22 * v.mY + m23 * v.mZ + m24,
		m31 * v.mX + m32 * v.mY + m33 * v.mZ + m34);
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
	return Vector4(
		m11 * v.mX + m12 * v.mY + m13 * v.mZ + m14 * v.mW,
		m21 * v.mX + m22 * v.mY + m23 * v.mZ + m24 * v.mW,
		m31 * v.mX + m32 * v.mY + m33 * v.mZ + m34 * v.mW,
		m41 * v.mX + m42 * v.mY + m43 * v.mZ + m44 * v.mW);
}

Matrix4 Matrix4::operator*(const Matrix4& m2) const
{
	Matrix4 res;
	res.m11 = m11 * m2.m11 + m12 * m2.m21 + m13 * m2.m31 + m14 * m2.m41;
	res.m12 = m11 * m2.m12 + m12 * m2.m22 + m13 * m2.m32 + m14 * m2.m42;
	res.m13 = m11 * m2.m13 + m12 * m2.m23 + m13 * m2.m33 + m14 * m2.m43;
	res.m14 = m11 * m2.m14 + m12 * m2.m24 + m13 * m2.m34 + m14 * m2.m44;

	res.m21 = m21 * m2.m11 + m22 * m2.m21 + m23 * m2.m31 + m24 * m2.m41;
	res.m22 = m21 * m2.m12 + m22 * m2.m22 + m23 * m2.m32 + m24 * m2.m42;
	res.m23 = m21 * m2.m13 + m22 * m2.m23 + m23 * m2.m33 + m24 * m2.m43;
	res.m24 = m21 * m2.m14 + m22 * m2.m24 + m23 * m2.m34 + m24 * m2.m44;

	res.m31 = m31 * m2.m11 + m32 * m2.m21 + m33 * m2.m31 + m34 * m2.m41;
	res.m32 = m31 * m2.m12 + m32 * m2.m22 + m33 * m2.m32 + m34 * m2.m42;
	res.m33 = m31 * m2.m13 + m32 * m2.m23 + m33 * m2.m33 + m34 * m2.m43;
	res.m34 = m31 * m2.m14 + m32 * m2.m24 + m33 * m2.m34 + m34 * m2.m44;

	res.m41 = m41 * m2.m11 + m42 * m2.m21 + m43 * m2.m31 + m44 * m2.m41;
	res.m42 = m41 * m2.m12 + m42 * m2.m22 + m43 * m2.m32 + m44 * m2.m42;
	res.m43 = m41 * m2.m13 + m42 * m2.m23 + m43 * m2.m33 + m44 * m2.m43;
	res.m44 = m41 * m2.m14 + m42 * m2.m24 + m43 * m2.m34 + m44 * m2.m44;
	return res;
}

const Matrix4& Matrix4::operator*=(float s)
{
	m11 *= s; m12 *= s; m13 *= s; m14 *= s;
	m21 *= s; m22 *= s; m23 *= s; m24 *= s;
	m31 *= s; m32 *= s; m33 *= s; m34 *= s;
	m31 *= s; m42 *= s; m33 *= s; m44 *= s;
	return *this;
}

const Matrix4& Matrix4::operator*=(const Matrix4& m2)
{
	return (*this) = (*this) * m2;
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics