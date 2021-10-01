#include "StringMethods.h"

using namespace std;

string pc::getString(bool value)
{
	return value ? "true" : "false";
}

string pc::getImprovedString(string s)
{
	if(s.length() == 1) s.append(".");
	while(s.length() < 4) s.append("0");
	return s;
}


string pc::getString(const Vector& point)
{
	return "(x: " + to_string(point.mX) + ", y: " + to_string(point.mY) + ", z: " + to_string(point.mZ) + ")";
}

string pc::getString(const Matrix& matrix)
{
	return "[--------------------------------]\n[ " +
		to_string(matrix.m11) + " | " + to_string(matrix.m12) + " | " + to_string(matrix.m13) + " ]\n" +
		"[--------------------------------]\n" +
		"[ " + to_string(matrix.m21) + " | " + to_string(matrix.m22) + " | " + to_string(matrix.m23) + " ]\n" +
		"[--------------------------------]\n" +
		"[ " + to_string(matrix.m31) + " | " + to_string(matrix.m32) + " | " + to_string(matrix.m33) + " ]\n" +
		"[--------------------------------]\n";
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0