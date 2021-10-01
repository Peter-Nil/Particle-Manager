#pragma once
#include "Vector.h"
#include "Entity.h"
#include "CollisionParticlePlane.h"
#include "Plane.h"

namespace pc
{
	class Constraint;

	class Particle : public Entity
    {
    protected:
        // Inverse of the mass of the particle. More useful to have objects with infinite mass (immovable) than zero mass (unstable).
        float mInverseMass;
        // Amount of damping applied to motion.
        float mDamping;
		float mActualDamp;
        Vector mPosition;
        // Previous position.
        Vector mPrevPosition;
        // Force to be applied at the next simulation iteration only.
        Vector mForce;
        // Delta time. Default is 1/60.
        float mDT;

    public:
		// Particle keeps track of its own constraints, to be able to delete them when removed.
		vector<Constraint*> mConstraints;

        Particle(Vector pos = Vector(), Vector vel = Vector(), Color color = GRAY, float mass = 0.2f, float damp = 0.1f, float dt = 1.0f / 60.0f);
        virtual ~Particle();

        // Simulate motion over time.
        virtual void update(float duration);
        virtual void draw(DemoHandler* draw, const bool drawDebug = false);
        // Clears the forces.
        void clearForces();
        void move(Vector deltaPos);

        // Adds the given force to the particle, to be applied at the next iteration only. 
        void addForce(const Vector& force);
        void addPosition(const Vector& move);

        CollisionParticlePlane getCollisionWithPlane(Plane* plane);
        // Gets/Sets the mass of the particle. This may not be zero.
        float getMass() const;
        void setMass(const float mass);
        // Gets/Sets the inverse mass of the particle. This may be zero.
        float getInverseMass() const;
        void setInverseMass(const float inverseMass);
        // Gets/Sets the current damping value.
        float getDamping() const;
        void setDamping(const float damping);
        // Gets/Sets the position of the particle.
		inline const Vector& getPosition() const { return mPosition; }
        void setPosition(const Vector& position);
        // Gets the previous position of the particle.
        inline const Vector& getPrevPosition() const { return mPrevPosition; }
        // Gets/Sets the velocity of the particle.
        Vector getVelocity() const;
        void setVelocity(const Vector& velocity);
		void setDeltaTime(float dt);
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009), the Cyclone physics system and Thomas Jakobsens Advanced Character Physics
// https://github.com/idmillington/cyclone-physics