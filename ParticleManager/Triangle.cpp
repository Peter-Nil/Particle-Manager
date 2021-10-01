#include "Triangle.h"

using namespace pc;

Triangle::Triangle(Vector p0, Vector p1, Vector p2)
{
	mPts.push_back(p0);
	mPts.push_back(p1);
	mPts.push_back(p2);
}

Triangle::Triangle(const Triangle &tri) : mPts(tri.mPts){}

Triangle::Triangle()
{
	mPts.push_back(Vector());
	mPts.push_back(Vector(1, 0, 0));
	mPts.push_back(Vector(0, 1, 0));
}


LineSegment Triangle::side(int nr)
{
	return LineSegment(mPts[nr % 3], mPts[(nr + 1) % 3]);
}

Vector Triangle::sideNorm(int nr)
{
	return (side(nr).direction() % polyNorm());
}

Vector Triangle::polyNorm()
{
	return ((mPts[1] - mPts[0]) % (mPts[2] - mPts[0]));
}

Vector Triangle::intersect(LineSegment ls)
{
	Vector norm = polyNorm();
	return ls.mP0 + ls.direction() * (((mPts[0] - ls.mP0) * norm) / (ls.direction() * norm)) ;
}

Vector Triangle::center()
{
	Vector r = mPts[0] + mPts[1] + mPts[2];
	return r * (1.0f / 3.0f);
}

Vector Triangle::project(Vector p)
{
	Vector norm = polyNorm();
	norm  *= ((p - mPts[0]) * norm) / (norm * norm);
	return p - norm;
}

double Triangle::area()
{
	// U x V is area of a parallelogram. Divide it by 2 and its the area of a triangle.
	return ((mPts[1] - mPts[0]) % (mPts[2] - mPts[0])).length()/2; 
}

bool Triangle::onPlane(Vector p)
{
	bool r = false;
	double con = polyNorm() * (p - mPts[0]);
	con *= con;
	if(con <= 0.001)
		r = true;
	return r;
}

bool Triangle::inPoly(Vector p)
{
	bool r = false;
	if(onPlane(p)){
		r = true;
		for(int i = 0; i < 3; i++)
			if(0 <= ((p - mPts[i]) * sideNorm(i)) || !side(i).onLineSegment(side(i).project(p))) // Checks if point is infront of side. Also checks if point cannot be projectet onto side.
				return false;
	}
	return r;
}

bool Triangle::inPoly(Vector p, Vector dir)
{
	const float EPSILON = 0.0000001f;
	Vector edge1, edge2, h, s, q;
	float a, f, u, v, t;

	edge1 = mPts[1] - mPts[0];
	edge2 = mPts[2] - mPts[0];
	h = dir % edge2;
	a = edge1 * h;
	if (-EPSILON < a && a < EPSILON) return false; // This ray is parallel to this triangle.
	f = 1.0f / a;
	s = p - mPts[0];
	u = f * (s * h);
	if (u < 0.0 || 1.0 < u) return false;
	q = s % edge1;
	v = f * (dir * q);
	if (v < 0.0 || 1.0 < u + v) return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	t = f * (edge2 * q);
	if (EPSILON < t && t < 1/EPSILON) return true; // Ray intersection.
	else return false; // This means that there is a line intersection but not a ray intersection.
}

void Triangle::move(Vector p)
{
	mPts[0] += p;
	mPts[1] += p;
	mPts[2] += p;
}

void Triangle::move(float x, float y, float z)
{
	move(Vector(x, y, z));
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0