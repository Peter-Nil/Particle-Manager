#include <assert.h>
#include "Particle.h"

using namespace pc;

Particle::Particle(Vector pos, Vector vel, Color color, float mass, float damp, float dt) : mPosition(pos)
{
    setVelocity(vel);
    setMass(mass);
	setDamping(damp);
	setDeltaTime(dt);
    mColor = color;
}

pc::Particle::~Particle(){}


void Particle::update(float dt)
{
    // We don't integrate things with zero mass.
    if (mInverseMass <= 0.0f) return;
    mDT = dt;
    // Update linear velocity and position from force and damping.
    Vector deltaPos = (mPosition - mPrevPosition) * mActualDamp + mForce * dt * dt * mInverseMass;
    mPrevPosition = mPosition;
    mPosition += deltaPos;
}

void Particle::draw(DemoHandler* draw, bool debug) 
{
    draw->drawPoint(mPosition.getPoint(), mColor);
    if (debug)
        draw->drawLine(mPosition.getPoint(), (mPosition + getVelocity().getNormal() * atanf(getVelocity().sqrLength())).getPoint(), RED);
}

void Particle::clearForces()
{
    mForce = Vector();
}

void Particle::move(Vector deltaPos)
{
    mPosition += deltaPos;
}


void Particle::addForce(const Vector& force)
{
    mForce += force;
}

void Particle::addPosition(const Vector& deltaPos)
{
    mPosition += deltaPos;
    mPrevPosition += deltaPos;
}


CollisionParticlePlane Particle::getCollisionWithPlane(Plane* plane)
{
    CollisionParticlePlane result = CollisionParticlePlane();

    // Gives positive penetration.
    float penetration = -plane->getDistance(mPosition);
    if (0 < penetration)
    {
        result.mPenetration = penetration;
        result.mRelativeVelocity = getVelocity() * plane->getNormal();
    }

    return result;
}


float Particle::getMass() const
{
    if (mInverseMass == 0)
        return FLT_MAX;
    else
        return 1.0f / mInverseMass;
}

void Particle::setMass(const float mass)
{
    assert(mass != 0);
    mInverseMass = 1.0f / mass;
}

float Particle::getInverseMass() const
{
    return mInverseMass;
}

void Particle::setInverseMass(const float inverseMass)
{
    mInverseMass = inverseMass;
}

float Particle::getDamping() const
{
    return mDamping;
}

void Particle::setDamping(const float damping)
{
    mDamping = damping;
	mActualDamp = powf(1 - damping, mDT);
}

void Particle::setPosition(const Vector& newPos)
{
    mPrevPosition += newPos - mPosition;
    mPosition = newPos;
}

Vector Particle::getVelocity() const
{
    return (mPosition - mPrevPosition) * (1.0f / mDT);
}

void Particle::setVelocity(const Vector& velocity)
{
    mPrevPosition = mPosition - velocity * mDT;
}

void Particle::setDeltaTime(float dt)
{
	mDT = dt;
	setDamping(mDamping);
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0
// Inspired by Henrik Engström, henrik.engstrom@his.se (2009), the Cyclone physics system and Thomas Jakobsens Advanced Character Physics
// https://github.com/idmillington/cyclone-physics