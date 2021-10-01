#pragma once
#include "Vector.h"
#include "DemoHandler.h"
#include "Entity.h"

namespace pc
{
	class Plane : public Entity
	{
		Vector mNormal;
		Vector mPosition;
	public:
		Plane(const Vector point, const Vector normal);

		// Distance From Point To Plane.
		const	float	getDistance(const Vector pt);
		const	Vector	getPointInPlane();
		const	Vector	getNormal();
		virtual void	draw(DemoHandler* draw, const bool drawDebug = false);

		// Plane keeps track of its own constraints, to be able to delete them when removed
		vector<Entity*> mConstraints;
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0