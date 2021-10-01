#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <typeinfo>
#include "Vector.h"
#include "Matrix.h"

namespace pc
{
	template<
		typename T, 
		typename = typename enable_if<
			is_arithmetic<T>::value ||
			is_base_of<T, string>::value
			, T>::type
	> std::string getString(T q)
	{
		std::ostringstream ss;
		ss << q;
		return ss.str();
	}
	std::string getString(bool value);
	std::string getString(const Vector& point);
	std::string getString(const Matrix& matrix);
	std::string getImprovedString(std::string s);
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0