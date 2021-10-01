#include "LineSegment.h"

using namespace pc;

LineSegment::LineSegment(Vector p0, Vector p1) : mP0(p0), mP1(p1){}

LineSegment::LineSegment(const LineSegment& ls) : mP0(ls.mP0), mP1(ls.mP1){}

LineSegment::LineSegment() : mP0(), mP1(Vector(1, 0, 0)){} // Right.

Vector LineSegment::direction()
{
	Vector dir(mP1 - mP0);
	return dir;
}

Vector LineSegment::center()
{
	Vector pos((mP0 + mP1) * 0.5);
	return pos;
}

Vector LineSegment::project(Vector p)
{ 
	Vector r = direction();
	r *= ((p - mP0) * r) / (r * r); // r is now mP0 local vector to nearest point on line to p.
	return mP0 + r; // Local origo + local vector = global vector.
}

double LineSegment::length()
{
	return (mP1 - mP0).length();
}

double LineSegment::sqrLength()
{
	Vector r = mP1 - mP0;
	return r*r;
}

double LineSegment::distance(Vector p)
{
	Vector proj = project(p);
	double distance = (proj - center()).sqrLength() * 2 <= sqrLength() ?
		(p - proj).length() :
		(mP0 - proj).sqrLength() < (mP1 - proj).sqrLength() ?
			(mP0 - p).length() :
			(mP1 - p).length(); // If everything else is false then the distance must be the length of mv1 to v.
	return distance;
}

bool LineSegment::onLineSegment(Vector p)
{
	bool a = false;
	if((p - center()).sqrLength() * 2 <= sqrLength() && (p - project(p)).sqrLength() < 0.001)
		a = true;
	return a;
}

void LineSegment::move(Vector v)
{
	mP0 += v;
	mP1 += v;
}

void LineSegment::move(float x, float y, float z)
{
	move(Vector(x, y, z));
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0