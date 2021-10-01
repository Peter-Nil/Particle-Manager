#pragma once
#include "Vector.h"
#include <cmath>

namespace pc
{
	class LineSegment
	{
	public:
		Vector mP0, mP1; // Position 1 and 2.

		LineSegment(Vector p0, Vector p1);
		LineSegment(const LineSegment& ls);
		LineSegment();
		Vector direction();
		Vector center();
		// Nearest point on line (but not necessary on line segment).
		Vector project(Vector v);
		double length();
		double sqrLength();
		// Nearest distance to line segment.
		double distance(Vector);
		bool onLineSegment(Vector);
		void move(Vector);
		void move(float, float, float);
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0