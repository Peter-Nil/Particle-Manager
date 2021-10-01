#pragma once
#include "Matrix4.h"

namespace pc
{
	// 3x3 matrix.
	class Matrix
	{
	public:
		// First rows then columns.
		float 
			m11, m12, m13,
			m21, m22, m23,
			m31, m32, m33;

		// Creates a zero matrix.
		Matrix();
		Matrix(float p11, float p12, float p13, float p21, float p22, float p23, float p31, float p32, float p33);
		// TODO is this a Rotational Matrix?
		Matrix(float a, float b, float g);
		// Uses the vectors as columns.
		Matrix(const Vector& col1, const Vector& col2, const Vector& col3);
		Matrix(const Matrix4& m);
		Matrix(const Matrix& m);

		void transpose();
		void invert();
		float determinant() const;
		Vector transformTranspose(const Vector& vector) const;
		
		// TODO.
		// Vector getColumn(unsigned i) const; 
		// Returns a new matrix containing the transpose of this matrix.
		Matrix getTranspose() const;
		// Returns a new matrix containing the inverse of this matrix.
		Matrix getInverse() const;
		// Sets the value of the matrix from inertia tensor values.
		void setInertiaTensorCoefficients(float ix, float iy, float iz, float ixy = 0, float ixz = 0, float iyz = 0);
		// Sets the value of the matrix as an inertia tensor of a rectangular block aligned with the body's coordinate system with the given axis half-sizes and mass.
		void setBlockInertiaTensor(const Vector &halfSizes, float mass);
		// Sets the matrix to be a skew symmetric matrix based on the given vector. The skew symmetric matrix is the equivalent of the vector product. So if a, b are vectors. a x b = A_s b where A_s is the skew symmetric form of a.
		void setSkewSymmetric(const Vector& v);

		// Does a componentwise addition of this matrix and the given matrix.
		Matrix operator+(const Matrix& m2) const;
		const Matrix& operator+=(const Matrix& m);
		// Does a componentwise subtraction of this matrix and the given matrix.
		Matrix operator-(const Matrix& m2) const;
		const Matrix& operator-=(const Matrix& m);
		// Multiplies this matrix in place by the given scalar.
		Matrix operator*(float s) const;
		// Used for linear transformations.
		Vector operator*(const Vector& v) const;
		// Used for linear transformations.
		Matrix operator*(const Matrix &m) const;
		const Matrix operator*=(float s);
		const Matrix& operator*=(const Matrix& m2);
	};
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics