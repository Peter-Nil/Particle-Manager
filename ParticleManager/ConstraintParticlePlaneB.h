#pragma once
#include "Particle.h"
#include "Plane.h"
#include "Constraint.h"

namespace pc
{
	// Can handle restitution.
	class ConstraintParticlePlaneB : public Constraint
	{
		Plane* mPlane;
	public:
		ConstraintParticlePlaneB(Particle* particle, Plane* plane)
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
				// Init.
				Vector contactNormal = mPlane->getNormal();
				float penetration = col.mPenetration;
				// TODO. Shall use whatever restitution is appropriate.
				float restitution = 0.5f;

				// Velocity change.
				if (col.mRelativeVelocity < 0)
				{
					// Calculates the new separating velocity.
					float deltaVelocity = -col.mRelativeVelocity * (1.0f + restitution);
					mParticles[0]->setVelocity(mParticles[0]->getVelocity() + contactNormal * deltaVelocity);
				}

				// Position change.
				Vector movePerIMass = contactNormal * penetration;
				mParticleMovement[0] = contactNormal * penetration;
				mParticles[0]->addPosition(mParticleMovement[0]);
			}
		}
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0
// Inspired by the Cyclone physics system https://github.com/idmillington/cyclone-physics