#pragma once
#include "Vector.h"
#include "Vector4.h"
#include <stdexcept>

namespace pc
{
	// 4x4 matrix.
	class Matrix4
	{
	public:
		float 
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44;

		// Creates an identity matrix.
		Matrix4();
		Matrix4(
			float p11, float p12, float p13, float p14,
			float p21, float p22, float p23, float p24,
			float p31, float p32, float p33, float p34,
			float p41, float p42, float p43, float p44);
		// Uses the vector4s as columns.
		Matrix4(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4);
		Matrix4(const Matrix4& m);

		void transpose();
		void invert();
		float determinant() const;
		// Transform the given vector by the transformational inverse of this matrix. Note this function relies on the fact that the inverse of a pure rotation matrix is its transpose. It separates the translational and rotation components, transposes the rotation, and multiplies out. If the matrix is not a scale and shear free transform matrix, then this function will not give correct results.
		Vector transformInvRot(const Vector& vector) const;
		
		// Gets a vector3 representing 3 first values from one column in the matrix.
		Vector getColumnVector(unsigned i) const;
		// TODO.
		// Vector4 getColumn(unsigned i) const; 
		// Returns a new matrix containing the transpose of this matrix.
		Matrix4 getTranspose() const;
		// Returns a new matrix containing the inverse of this matrix.
		Matrix4 getInverse() const;
		
		// Does a componentwise addition of this matrix and the given matrix.
		Matrix4 operator+(const Matrix4& m2) const;
		const Matrix4& operator+=(const Matrix4& m2);
		// Does a componentwise subtraction of this matrix and the given matrix.
		Matrix4 operator-(const Matrix4& m2) const;
		const Matrix4& operator-=(const Matrix4& m2);
		// Multiplies this matrix in place by the given scalar.
		Matrix4 operator*(float s) const;
		// Used for linear transformations.
		Vector operator*(const Vector& v) const;
		// Used for linear transformations.
		Vector4 operator*(const Vector4& v) const;
		// Used for linear transformations.
		Matrix4 operator*(const Matrix4& m2) const;
		const Matrix4& operator*=(float s);
		const Matrix4& operator*=(const Matrix4& m2);
	};
}

// Written by Peter Nilssonand Christian Larsson, licensed under CC BY - SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009) and by the Cyclone physics system
// https://github.com/idmillington/cyclone-physics