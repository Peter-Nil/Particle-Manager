#pragma once
#include "Particle.h"
#include "Plane.h"
#include "Constraint.h"

namespace pc
{
	// Cant handle restitution.
	class ConstraintParticlePlaneA : public Constraint
	{
		Plane* mPlane;
	public:
		ConstraintParticlePlaneA(Particle* particle, Plane* plane)
		{
			mParticles[0] = particle;
			mParticles[1] = nullptr;
			mPlane = plane;
		}
		

		// Gets positive displacement.  
		virtual float getDisplacement() const
		{
			float dis = -mPlane->getDistance(mParticles[0]->getPosition());
			return dis < 0 ? 0 : dis;
		}

		// Gets positive square displacement.
		virtual float getSqrDisplacement() const
		{
			float dis = mPlane->getDistance(mParticles[0]->getPosition());
			return dis * dis;
		}

		virtual Vector getNormal() const
		{
			return mPlane->getNormal();
		}

		virtual Vector getScaledNormal() const
		{
			return mPlane->getNormal() * -mPlane->getDistance(mParticles[0]->getPosition());
		}

		virtual float getSeparatingVelocity() const
		{
			return mParticles[0]->getVelocity() * mPlane->getNormal();
		}

		virtual Vector getRelativeVelocity() const
		{
			return mParticles[0]->getVelocity();
		}

		virtual void applyConstraint(float dt)
		{
			CollisionParticlePlane col = mParticles[0]->getCollisionWithPlane(mPlane);
			mParticleMovement[0] = Vector();
			if (0 < col.mPenetration)
			{
				mParticleMovement[0] = mPlane->getNormal() * col.mPenetration;
				mParticles[0]->move(mParticleMovement[0]);
			}
		}
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0