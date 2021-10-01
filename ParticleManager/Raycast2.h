#pragma once
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include "Vector.h"
#include "ParticleCuboid.h"

namespace pc
{
	class Vector2
	{
	public:
		float mX;
		float mY;
	};

	class Polygon2
	{
	public:
		vector<Vector2> mVertices;
	};

	class Raycast2
	{
		Polygon2 mPolygon;
		float mRed;
		float mBlue;
		bool checkEdge(Vector2* edgeStart, Vector2* edgeEnd, Vector2 testCollisionPoint);
		bool checkPolygon(const std::vector<Vector2>& vectors, Vector2 testCollisionPoint);
	public:
		bool performRaycast(vector<ParticleCuboid*>& entities, Vector2 rayStartPos);
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0