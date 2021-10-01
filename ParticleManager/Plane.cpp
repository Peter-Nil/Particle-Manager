#include "Plane.h"

using namespace pc;

Plane::Plane(const Vector point, const Vector normal)
{
	mPosition	= point;
	mNormal		= normal;
	mNormal.normalize();	// Better safe than sorry.
}

const float Plane::getDistance(const Vector pt)
{
	Vector p_q = pt - mPosition;
	return p_q * mNormal;
}

const Vector Plane::getPointInPlane()
{
	return mPosition;
}

const Vector Plane::getNormal()
{
	return mNormal;
}

void Plane::draw(DemoHandler* draw, const bool drawDebug)
{
	draw->drawPlane(mPosition.getPoint(), mNormal.getPoint(), GREEN, 15); // Draws a green square representing the plane.

	if(drawDebug)
		draw->drawLine(mPosition.getPoint(), (mPosition + mNormal).getPoint(), PINK); // Draws a pink normal.

	// Draws green lines representing the plane in x-y projection.
	Vector lineDir = Vector(0, 0, 1) % mNormal; // Cross product of the normal and z axel gives us a line going along the plane from a x-y view.
	lineDir.normalize();
	draw->drawLine((mPosition - lineDir * 20).getPoint(), (mPosition + lineDir * 20).getPoint(), GREEN); 
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0