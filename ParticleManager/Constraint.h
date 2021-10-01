#pragma once
#include "Entity.h"
#include "Particle.h"

namespace pc
{
	class Constraint : public Entity
	{
	public:
		// Holds a particle or a pair of particles.
		Particle*		mParticles[2];
		Vector			mParticleMovement[2];
		
		virtual float	getSeparatingVelocity() const	= 0;
		virtual Vector	getRelativeVelocity() const		= 0;
		virtual float	getDisplacement() const			= 0;
		virtual float	getSqrDisplacement() const		= 0;
		virtual Vector	getNormal() const				= 0;
		virtual Vector	getScaledNormal() const			= 0;
		virtual void	applyConstraint(float dt)		= 0;
		virtual void	draw(DemoHandler* draw, const bool drawDebug = false){}
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0