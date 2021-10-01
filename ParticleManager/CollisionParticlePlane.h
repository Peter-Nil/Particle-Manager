#pragma once
#include "Vector.h"

namespace pc
{
	class CollisionParticlePlane
	{
	public:
		float mPenetration; // Penetration depth.
		float mRelativeVelocity; // Relative velocity.

		CollisionParticlePlane(float penetrationDepth = 0.0f, float relativeVelocity = 0.0f) :
			mPenetration(penetrationDepth), mRelativeVelocity(relativeVelocity) {}
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0