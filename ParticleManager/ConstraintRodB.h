#pragma once
#include "Constraint.h"
#include "Particle.h"

namespace pc
{
	// Can handle restitution/elasticity. TODO. But diverges from ConstraintRodA and thats why restitution is commented out in order to figure out whats wrong first.
	class ConstraintRodB : public ConstraintRod
	{
		// Holds the length of the rod.
		const float mRestLength;
		const float mSqrLength;

		/*float calculateCurrentLength() 
		{
			return mCurrentLen = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).length();
		}*/
	public:
		// Negative length is handled as positive.
		ConstraintRodB(Particle* particle1, Particle* particle2, float length, Color color = GRAY) : mRestLength(length >= 0.0f ? length : -length), mSqrLength(length >= 0.0f ? length*length : -length*length)
		{
			mParticles[0] = particle1;
			mParticles[1] = particle2;
			mColor = color;
		}

		ConstraintRodB(Particle* particle1, Particle* particle2, Color color = GRAY) : mRestLength((particle1->getPosition() - particle2->getPosition()).length()), mSqrLength((particle1->getPosition() - particle2->getPosition()).sqrLength())
		{
			mParticles[0] = particle1;
			mParticles[1] = particle2;
			mColor = color;
		}

		//ConstraintRodB(pair<Particle*, Particle*> particlePair) : ConstraintRodB(particlePair.first, particlePair.second) {}
		//ConstraintRodB(pair<Particle*, Particle*> particlePair, float length) : ConstraintRodB(particlePair.first, particlePair.second, length) {}


		virtual float getSeparatingVelocity() const
		{
			Vector normal = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).getNormal();
			return (mParticles[0]->getVelocity() - mParticles[1]->getVelocity()) * normal;
		}

		virtual Vector getRelativeVelocity() const
		{
			return mParticles[0]->getVelocity() - mParticles[1]->getVelocity();
		}

		virtual float getDisplacement() const
		{
			float currentLength = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).length();

			float displacement = 0.0f;
			if (currentLength > mRestLength)
				displacement = currentLength - mRestLength;
			else
				displacement = mRestLength - currentLength;
			return displacement;
		}

		virtual float getSqrDisplacement() const
		{
			float currentSqrLength = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).sqrLength();
			float sqrDisplacement = 0.0f;
			if (currentSqrLength > mSqrLength)
				sqrDisplacement = currentSqrLength - mSqrLength;
			else
				sqrDisplacement = mSqrLength - currentSqrLength;
			return sqrDisplacement;
		}

		virtual Vector getNormal() const
		{
			return (mParticles[1]->getPosition() - mParticles[0]->getPosition()).getNormal();
		}

		virtual Vector getScaledNormal() const
		{
			return mParticles[1]->getPosition() - mParticles[0]->getPosition();
		}

		virtual void applyConstraint(float dt)
		{
            /// Init.
			// Check if we're over-extended.
			float currentLen = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).length();
			// The (contact) normal depends on whether we're extending or compressing.
			Vector contactNormal = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).getNormal();
			// Get positive displacement and set direction of normal.
			float displacement = 0.0f;
			if (currentLen > mRestLength)
				displacement = currentLen - mRestLength;
			else
			{
				contactNormal = contactNormal * -1;
				displacement = mRestLength - currentLen;
			}
			if (currentLen == mRestLength)
				return;
			// Get the total inverse mass for both particles. The movement of each particle is proportional to their inverse mass divided by total inverse mass.
            float totalInverseMass = mParticles[0]->getInverseMass() + mParticles[1]->getInverseMass();
            // If all particles have infinite mass, then we do nothing because impulses and interpenetration have no effect.
            if (totalInverseMass <= 0)
                return;


            /// Resolve Velocity. Handles the impulse calculations for this collision.
            // Find the velocity in the direction of the contact. 
            float separatingVelocity = (mParticles[0]->getVelocity() - mParticles[1]->getVelocity()) * contactNormal;
            // Find the amount of impulse per unit of inverse mass. Negative seperating velocity is deltavelocity.
            Vector impulsePerIMass = contactNormal * (-separatingVelocity / totalInverseMass);
			// Elastic version.
			//Vector impulsePerIMass = normal * (-separatingVelocity * (1 + mManager->mElastic) / totalInverseMass);
            // Apply impulses: they are applied in the direction of the contact, and are proportional to the inverse mass. The other particle goes in the opposite direction.
            mParticles[0]->setVelocity(mParticles[0]->getVelocity() + impulsePerIMass *  mParticles[0]->getInverseMass());
            mParticles[1]->setVelocity(mParticles[1]->getVelocity() + impulsePerIMass * -mParticles[1]->getInverseMass());


            /// Resolve Interpenetration.
            // If we don't have any displacement then stop.
            if (displacement <= 0)
                return;
            // Find the amount of displacement per inverse mass and give it a contact direction.
            Vector movePerIMass = contactNormal * (displacement / totalInverseMass);
            // Saves the positions changes: they are proportional to their inverse mass. The other particle goes in the opposite direction.
			mParticleMovement[0] = movePerIMass *  mParticles[0]->getInverseMass();
			mParticleMovement[1] = movePerIMass * -mParticles[1]->getInverseMass();
            // Apply the positions changes.
            mParticles[0]->addPosition(mParticleMovement[0]);
            mParticles[1]->addPosition(mParticleMovement[1]);
		}

		Vector getCenterPosition() const
		{
			Vector particleFirstPosition	= mParticles[0]->getPosition();
			Vector particleSecondPosition	= mParticles[1]->getPosition();

			float midX = (particleFirstPosition.mX + particleSecondPosition.mX) / 2;
			float midY = (particleFirstPosition.mY + particleSecondPosition.mY) / 2;
			float midZ = (particleFirstPosition.mZ + particleSecondPosition.mZ) / 2;

			return Vector(midX, midY, midZ);
		}

        virtual void draw(DemoHandler* draw, const bool drawDebug = false) 
        {
            draw->drawLine(mParticles[0]->getPosition().getPoint(), mParticles[1]->getPosition().getPoint(), mColor);
        }
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009), the Cyclone physics system and Thomas Jakobsens Advanced Character Physics
// https://github.com/idmillington/cyclone-physics