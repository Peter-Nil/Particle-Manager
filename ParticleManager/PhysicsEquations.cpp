#include "PhysicsEquations.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace pc;

Vector pc::airReistanceAcceleration(Vector velocity, float airReistanceCoefficient1, float airReistanceCoefficient2, float mass) 
{
	float velocityMagnitude = velocity.length();
	return velocity * -(airReistanceCoefficient1 + airReistanceCoefficient2 * velocityMagnitude / mass);
}

Vector pc::newtonsLawOfUniversalGravitation(float mass1, float mass2, Vector distanceVectorFromMass1ToMass2) 
{
	float gravitationalConstant = 6.674f * powf(10., -11.);
	float squaredScalarDistance = powf(distanceVectorFromMass1ToMass2.length(), 2.);
	//cout << to_string(mass1) << "\n";
	distanceVectorFromMass1ToMass2.normalize();
	Vector forceDirection = distanceVectorFromMass1ToMass2;
	//cout << to_string(gravitationalConstant*(mass1 * mass2) / (squaredScalarDistance)) << "\n";
	return forceDirection*(gravitationalConstant*(mass1 * mass2) / (squaredScalarDistance));
}

Vector pc::centripetalForce(float mass, Vector angularVelocity, Vector distanceFromCenter) 
{
	Vector crossProducts = angularVelocity.operator%(distanceFromCenter.operator%(distanceFromCenter));
	return crossProducts * mass;
}

float pc::getSpeedFromCentripetalForce(float mass, Vector centripetalForce, Vector distanceFromCenter) 
{
	float dotProduct = centripetalForce * distanceFromCenter;
	return sqrtf(dotProduct / mass);
}

Vector pc::newtonsLawOfUniversalGravitationGetAcceleration(float mass1, float mass2, Vector distanceVectorFromMass1ToMass2) 
{
	float gravitationalConstant = 6.674f * powf(10., -11.);
	float squaredScalarDistance = powf(distanceVectorFromMass1ToMass2.length(), 2.);
	//cout << to_string(mass1) << "\n";
	distanceVectorFromMass1ToMass2.normalize();
	Vector forceDirection = distanceVectorFromMass1ToMass2;
	//cout << to_string(gravitationalConstant*(mass1 * mass2) / (squaredScalarDistance)) << "\n";
	return forceDirection * (gravitationalConstant*(mass2) / (squaredScalarDistance));
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0