#include "Matrix.h"

using namespace pc;

Matrix::Matrix()
{
	m11 = m12 = m13 = 
	m21 = m22 = m23 = 
	m31 = m32 = m33 = 0;	
}

Matrix::Matrix(float p11, float p12, float p13, float p21, float p22, float p23, float p31, float p32, float p33)
{
	m11 = p11; m12 = p12; m13 = p13;
	m21 = p21; m22 = p22; m23 = p23;
	m31 = p31; m32 = p32; m33 = p33;
}

Matrix::Matrix(float a, float b, float g)
{
	float DtR = 3.1415926f / 180.0f; // 1 degree to radian.
	float sinA = sin(a * DtR), cosA = cos(a * DtR);
	float sinB = sin(b * DtR), cosB = cos(b * DtR);
	float sinG = sin(g * DtR), cosG = cos(g * DtR);

	m11 = cosA * cosB;
	m12 = cosA * sinB * sinG - sinA * cosG;
	m13 = cosA * sinB * cosG + sinA * sinG;
	m21 = sinA * cosB;
	m22 = sinA * sinB * sinG + cosA * cosG;
	m23 = sinA * sinB * cosG - cosA * sinG;
	m31 = -sinB;
	m32 = cosB * sinG;
	m33 = cosB * cosG;
}

Matrix::Matrix(const Vector& col1, const Vector& col2, const Vector& col3)
{
	m11 = col1.mX; m12 = col2.mX; m13 = col3.mX;
	m21 = col1.mY; m22 = col2.mY; m23 = col3.mY;
	m31 = col1.mZ; m32 = col2.mZ; m33 = col3.mZ;
}

Matrix::Matrix(const Matrix4& m) 
{
	m11 = m.m11; m12 = m.m12; m13 = m.m13;
	m21 = m.m21; m22 = m.m22; m23 = m.m23;
	m31 = m.m31; m32 = m.m32; m33 = m.m33;
}

Matrix::Matrix(const Matrix& m)
{
	m11 = m.m11; m12 = m.m12; m13 = m.m13;
	m21 = m.m21; m22 = m.m22; m23 = m.m23;
	m31 = m.m31; m32 = m.m32; m33 = m.m33;
}


void Matrix::transpose()
{
	Matrix tmp = (*this);
	m12 = tmp.m21;
	m13 = tmp.m31;
	m21 = tmp.m12;
	m23 = tmp.m32;
	m31 = tmp.m13;
	m32 = tmp.m23;
}

void Matrix::invert()
{
	float det = determinant();
	if (det * det < 0.000001f) return;
	float nm11 = (m22 * m33 - m23 * m32) / det;
	float nm12 = (m13 * m32 - m12 * m33) / det;
	float nm13 = (m12 * m23 - m13 * m22) / det;
	float nm21 = (m23 * m31 - m21 * m33) / det;
	float nm22 = (m11 * m33 - m13 * m31) / det;
	float nm23 = (m13 * m21 - m11 * m23) / det;
	float nm31 = (m21 * m32 - m22 * m31) / det;
	float nm32 = (m12 * m31 - m11 * m32) / det;
	float nm33 = (m11 * m22 - m12 * m21) / det;
	m11 = nm11; m12 = nm12; m13 = nm13;
	m21 = nm21; m22 = nm22; m23 = nm23;
	m31 = nm31; m32 = nm32; m33 = nm33;
}

float Matrix::determinant() const
{
	return 
		m11 * m22 * m33 + 
		m12 * m23 * m31 + 
		m13 * m21 * m32 - 
		m11 * m23 * m32 - 
		m12 * m21 * m33 - 
		m13 * m22 * m31;
}

Vector Matrix::transformTranspose(const Vector& vector) const
{
	Matrix transpose = Matrix(*this);
	transpose.transpose();
	return transpose * vector;
}


Matrix Matrix::getTranspose() const
{
	Matrix result = Matrix(*this);
	result.transpose();
	return result;
}

Matrix Matrix::getInverse() const
{
	Matrix result = Matrix(*this);
	result.invert();
	return result;
}

void Matrix::setInertiaTensorCoefficients(float ix, float iy, float iz, float ixy, float ixz, float iyz)
{
	m11 = ix;
	m12 = m21 = -ixy;
	m13 = m31 = -ixz;
	m22 = iy;
	m23 = m32 = -iyz;
	m33 = iz;
}

void Matrix::setBlockInertiaTensor(const Vector &halfSizes, float mass)
{
	Vector squares = halfSizes.componentProduct(halfSizes);
	setInertiaTensorCoefficients(
		0.3f * mass * (squares.mY + squares.mZ),
		0.3f * mass * (squares.mX + squares.mZ),
		0.3f * mass * (squares.mX + squares.mY));
}

void Matrix::setSkewSymmetric(const Vector& v)
{
	m11 = m22 = m33 = 0;
	m12 = -v.mZ;
	m13 = +v.mY;
	m21 = +v.mZ;
	m23 = -v.mX;
	m31 = -v.mY;
	m32 = +v.mX;
}


Matrix Matrix::operator+(const Matrix& m2) const
{
	Matrix res = Matrix(*this);
	res.m11 += m2.m11; res.m12 += m2.m12; res.m13 += m2.m13;
	res.m21 += m2.m21; res.m22 += m2.m22; res.m23 += m2.m23;
	res.m31 += m2.m31; res.m32 += m2.m32; res.m33 += m2.m33;
	return res;
}

const Matrix& Matrix::operator+=(const Matrix& m2)
{
	m11 += m2.m11; m12 += m2.m12; m13 += m2.m13;
	m21 += m2.m21; m22 += m2.m22; m23 += m2.m23;
	m31 += m2.m31; m32 += m2.m32; m33 += m2.m33;
	return *this;
}

Matrix Matrix::operator-(const Matrix& m2) const
{
	Matrix res = Matrix(*this);
	res.m11 -= m2.m11; res.m12 -= m2.m12; res.m13 -= m2.m13;
	res.m21 -= m2.m21; res.m22 -= m2.m22; res.m23 -= m2.m23;
	res.m31 -= m2.m31; res.m32 -= m2.m32; res.m33 -= m2.m33;
	return res;
}

const Matrix& Matrix::operator-=(const Matrix& m2)
{
	m11 -= m2.m11; m12 -= m2.m12; m13 -= m2.m13;
	m21 -= m2.m21; m22 -= m2.m22; m23 -= m2.m23;
	m31 -= m2.m31; m32 -= m2.m32; m33 -= m2.m33;
	return *this;
}

Matrix Matrix::operator*(float s) const
{
	Matrix res = Matrix(*this);
	res.m11 *= s; res.m12 *= s; res.m13 *= s;
	res.m21 *= s; res.m22 *= s; res.m23 *= s;
	res.m31 *= s; res.m32 *= s; res.m33 *= s;
	return res;
}

Vector Matrix::operator*(const Vector& v) const
{
	return Vector(
		m11 * v.mX + m12 * v.mY + m13 * v.mZ,
		m21 * v.mX + m22 * v.mY + m23 * v.mZ,
		m31 * v.mX + m32 * v.mY + m33 * v.mZ);
}

Matrix Matrix::operator*(const Matrix &m2) const
{
	Matrix res;
	res.m11 = m11 * m2.m11 + m12 * m2.m21 + m13 * m2.m31;
	res.m12 = m11 * m2.m12 + m12 * m2.m22 + m13 * m2.m32;
	res.m13 = m11 * m2.m13 + m12 * m2.m23 + m13 * m2.m33;

	res.m21 = m21 * m2.m11 + m22 * m2.m21 + m23 * m2.m31;
	res.m22 = m21 * m2.m12 + m22 * m2.m22 + m23 * m2.m32;
	res.m23 = m21 * m2.m13 + m22 * m2.m23 + m23 * m2.m33;

	res.m31 = m31 * m2.m11 + m32 * m2.m21 + m33 * m2.m31;
	res.m32 = m31 * m2.m12 + m32 * m2.m22 + m33 * m2.m32;
	res.m33 = m31 * m2.m13 + m32 * m2.m23 + m33 * m2.m33;
	return res;
}

const Matrix Matrix::operator*=(float s)
{
	m11 *= s; m12 *= s; m13 *= s;
	m21 *= s; m22 *= s; m23 *= s;
	m31 *= s; m32 *= s; m33 *= s;
	return *this;
}

const Matrix& Matrix::operator*=(const Matrix& m2)
{
	return (*this) = (*this) * m2;
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics