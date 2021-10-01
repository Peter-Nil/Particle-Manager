#pragma once
#include "LineSegment.h"
#include <vector>

namespace pc
{
	class Triangle
	{
	public:
		std::vector<Vector> mPts; // Points.

		Triangle(Vector, Vector, Vector);
		Triangle(const Triangle &p);
		Triangle();

		// Side 0 -> 1 is nr = 0 etc.
		LineSegment side(int nr);
		// Does not have length 1. TODO The Side normals shall always point out from triangle.
		Vector sideNorm(int);
		// Does not have a length of 1. TODO Check if it works.
		Vector polyNorm();
		Vector intersect(LineSegment);
		Vector center();
		// Closest vector on plane.
		Vector project(Vector);
		double area();
		// Checks if point is inside triangle.
		bool inPoly(Vector);
		// TODO. Check if a ray(half line) intersects the triangle. Uses Möller–Trumbore intersection algorithm: 
		// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
		bool inPoly(Vector, Vector);
		bool onPlane(Vector);
		void move(Vector);
		void move(float, float, float);
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0