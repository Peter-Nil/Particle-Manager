#pragma once
#include "Constraint.h"
#include "Particle.h"

namespace pc
{
	class ConstraintRodA : public ConstraintRod
	{
		// Holds the length of the rod.
		const float mRestLength;
		const float mSqrLength;
	public:
		// Negative length is handled as positive.
		ConstraintRodA(Particle* particle1, Particle* particle2, float length, Color color = GRAY) : mRestLength(length >= 0.0f ? length : -length), mSqrLength(length >= 0.0f ? length*length : -length*length) 
		{
			mParticles[0] = particle1;
			mParticles[1] = particle2;
			mColor = color;
		}

		ConstraintRodA(Particle* particle1, Particle* particle2, Color color = GRAY) : mRestLength((particle1->getPosition() - particle2->getPosition()).length()), mSqrLength((particle1->getPosition() - particle2->getPosition()).sqrLength())
		{
			mParticles[0] = particle1;
			mParticles[1] = particle2;
			mColor = color;
		}

		ConstraintRodA(pair<Particle*, Particle*> particlePair, Color color = GRAY) : ConstraintRodA(particlePair.first, particlePair.second, color) {}
		ConstraintRodA(pair<Particle*, Particle*> particlePair, float length, Color color = GRAY) : ConstraintRodA(particlePair.first, particlePair.second, length, color) {}

		float getSeparatingVelocity() const
		{
			Vector ContactNormal = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).getNormal();
			return (mParticles[0]->getVelocity() - mParticles[1]->getVelocity()) * ContactNormal;
		}

		virtual Vector getRelativeVelocity() const
		{
			Vector relativeVelocity = mParticles[0]->getVelocity() - mParticles[1]->getVelocity();
			return relativeVelocity;
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
			float CurrentSqrLen = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).sqrLength();
			float sqrDisplacement = 0.0f;
			if (CurrentSqrLen > mSqrLength)
				sqrDisplacement = CurrentSqrLen - mSqrLength;
			else
				sqrDisplacement = mSqrLength - CurrentSqrLen;
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
			float CurrentLen = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).length();
            // If we don't have any displacement, skip.
			if (CurrentLen == mRestLength)
				return;
			Vector ContactNormal = (mParticles[1]->getPosition() - mParticles[0]->getPosition()).getNormal();
			float displacement = CurrentLen - mRestLength;
			// Get the total inverse mass for both particles. The movement of each particle is proportional to their inverse mass divided by total inverse mass.
            float totalInverseMass = mParticles[0]->getInverseMass() + mParticles[1]->getInverseMass();
            // If all particles have infinite mass, then we do nothing because impulses and interpenetration have no effect.
            if (totalInverseMass <= 0)
                return;

            /// Resolve Interpenetration.
            // Find the amount of displacement per inverse mass and give it a contact direction.
            Vector movePerIMass = ContactNormal * (displacement / totalInverseMass);
            // Saves the positions changes: they are proportional to their inverse mass. The other particle goes in the opposite direction.
			mParticleMovement[0] = movePerIMass *  mParticles[0]->getInverseMass();
			mParticleMovement[1] = movePerIMass * -mParticles[1]->getInverseMass();
            // Apply the positions changes.
            mParticles[0]->move(mParticleMovement[0]);
            mParticles[1]->move(mParticleMovement[1]);
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