#pragma once
#include "Constraint.h"
#include "Particle.h"

namespace pc
{
	class ConstraintRod : public Constraint 
	{
	public:
		virtual Vector getCenterPosition() const = 0;
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0