#pragma once
#include "Vector.h"

namespace pc
{
	Vector	airReistanceAcceleration(Vector velocity, float airReistanceCoefficient1, float airReistanceCoefficient2, float mass);
	Vector	newtonsLawOfUniversalGravitation(float mass1, float mass2, Vector distanceVectorFromMass1ToMass2);
	Vector	centripetalForce(float mass, Vector angularVelocity, Vector distanceFromCenter);
	float	getSpeedFromCentripetalForce(float mass, Vector centripetalForce, Vector distanceFromCenter);
	Vector	newtonsLawOfUniversalGravitationGetAcceleration(float mass1, float mass2, Vector distanceVectorFromMass1ToMass2);
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0