#pragma once
// This class is used to represent a fixed point in 3D-space. 
// It is deliberately simple as you should implement a proper Vector class
// for your calculations. Use this only in the communication with the DemoHandler.

namespace pc
{
	class Point
	{
	public:
		float mX, mY, mZ;
		Point()
		{
			mX = mY = mZ = 0;
		}
		Point(const float X, const float Y, const float Z)
		{
			mX = X;
			mY = Y;
			mZ = Z;
		}
	};
}

// Original work by Henrik Engström, henrik.engstrom@his.se (2009)
// Modified work by Peter Nilsson and Christian Larsson licensed under CC BY-SA 4.0