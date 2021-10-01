#include "ParticleManager.h"
#include <iostream>
using std::vector;

using namespace pc;

ParticleManager::ParticleManager(){}

ParticleManager::~ParticleManager(){ clearAll(); }


void ParticleManager::clearAll()
{
	// Delete all.
	for(unsigned i = 0; i < mEntities.size(); i++) 
		delete mEntities[i];
	// Clear all.
	mEntities.clear();
	mParticles.clear();
	mConstraints.clear();
	mCuboids.clear();
	mPlanes.clear();
	mRagdolls.clear();
}

void ParticleManager::update(float dt)
{
	assert(dt > 0.0);
	runPhysics(dt);
	manageCollisions();
	
	if (mRecord)
	{
		mBenchmarker.setStartTimeToNow();
		(this->*mApplyConstraintsPtr)(dt);
		mBenchmarker.storeDuration();
		mBenchmarker.storeStabilityError(getStabilityError());		
		mBenchmarks++;
		if (mBenchmarks > 99) 
			stopRecord(true);
	}
	else
		(this->*mApplyConstraintsPtr)(dt);
}

void ParticleManager::runPhysics(float deltaTime)
{
	// Add gravitation forces.
	if(mIsGravityActive)
		for(auto particle : mParticles)
			particle->addForce(mGravityAcceleration * particle->getMass());

	// Update state by integration.
	for(auto entity : mEntities)
		entity->update(deltaTime);

	// Clear forces.
	for (auto particle : mParticles)
		particle->clearForces();
}

void ParticleManager::manageCollisions()
{
	// Check collision for rigidbodies with the planes.
	for(unsigned i = 0; i < mPlanes.size(); i++)
		for(auto particle : mParticles)
			manageCollisionParticlePlane(mPlanes[i], particle);
}

void ParticleManager::manageCollisionParticlePlane(Plane* plane, Particle* particle)
{
	CollisionParticlePlane collision = particle->getCollisionWithPlane(plane);

	if(collision.mPenetration > 0 && collision.mRelativeVelocity < 0)
	{
		Vector normal = plane->getNormal();
		float j = -(1 + mElasticity) * collision.mRelativeVelocity;
		particle->addPosition(normal * collision.mPenetration * (1 + mElasticity));
		particle->setVelocity(particle->getVelocity() + normal * j);
	}
}

void ParticleManager::draw(DemoHandler* draw)
{
	switch (mDrawState)
	{
	case DRAW_NORMAL:
	case DRAW_NORMAL_WITHOUT_GUI:
		for (auto drawables : mEntities)
			drawables->draw(draw, false);
		break;
	case DRAW_DEBUG:
		for (auto drawables : mEntities)
			drawables->draw(draw, true);
		break;
	case DRAW_SKIN_ONLY:
		for (auto drawables : mRagdolls)
			drawables->draw(draw, false);
		break;
	case NO_DRAWING:
		break;
	}

	if (mRecord)
	{
		mBenchmarker.saveScreenshotOfWindow();
	}
}

void ParticleManager::setConstraintSolverType(int index)
{
	mConstraintSolverType = index;

	switch (mConstraintSolverType)
	{
	case 1: mApplyConstraintsPtr = &ParticleManager::applyConstraintsV1; break;
	case 2: mApplyConstraintsPtr = &ParticleManager::applyConstraintsV2; break;
	case 3: mApplyConstraintsPtr = &ParticleManager::applyConstraintsV3; break;
	case 4: mApplyConstraintsPtr = &ParticleManager::applyConstraintsV4; break;
	}
}

std::vector<float>* ParticleManager::getStabilityError() 
{
	std::vector<float> *errors = new std::vector<float>;
	for (unsigned i = 0; i < mConstraints.size(); i++)
		errors->push_back(mConstraints[i]->getDisplacement());
	return errors;
}


void ParticleManager::applyConstraintsV1(float dt)
{
	for (unsigned j = 0; j < mRelaxationConstraints; j++)
		for (unsigned i = 0; i < mConstraints.size(); i++)
			mConstraints[i]->applyConstraint(dt);
}

void ParticleManager::applyConstraintsV2(float dt)
{
	const unsigned iterations = mConstraints.size() * mRelaxationConstraints;
	const unsigned numContacts = mConstraints.size();

	for(unsigned iterationsUsed = 0; iterationsUsed < iterations; iterationsUsed++)
	{
		// Find the constraint with the largest closing velocity.
		float max = 0;
		unsigned maxIndex = numContacts;
		for (unsigned i = 0; i < numContacts; i++)
		{
			float sqrDis = mConstraints[i]->getDisplacement();
			if (sqrDis > max)
			{
				max = sqrDis;
				maxIndex = i;
			}
		}
		// Do we have anything worth resolving?
		if (maxIndex == numContacts) return;
		// Apply this constraint.
		mConstraints[maxIndex]->applyConstraint(dt);
	}
}

void ParticleManager::applyConstraintsV3(float dt)
{
	// TODO make this constraint solver more like ians constraint solver.
	const unsigned iterations = mConstraints.size() * mRelaxationConstraints;
	const unsigned numContacts = mConstraints.size();
	float* displacements = new float[numContacts];
	Vector* contactNormals = new Vector[numContacts];

	int sizeOfDisp = sizeof(displacements);

	for (unsigned i = 0; i < numContacts; i++)
	{
		displacements[i] = mConstraints[i]->getDisplacement();
		contactNormals[i] = mConstraints[i]->getNormal();
	}

	for (unsigned iterationsUsed = 0; iterationsUsed < iterations; iterationsUsed++)
	{
		// Find the constraint with the largest seperating velocity.
		float min = FLT_MAX;
		unsigned minIndex = numContacts;
		for (unsigned i = 0; i < numContacts; i++)
		{
			float cloVel = mConstraints[i]->getRelativeVelocity() * contactNormals[i];
			if (cloVel < min && displacements[i] > 0)
			{
				min = cloVel;
				minIndex = i;
			}
		}
		// Do we have anything worth resolving?
		if (minIndex == numContacts) return;
		// Apply this constraint.
		mConstraints[minIndex]->applyConstraint(dt);

		// Update the interpenetrations for all particles.
		Vector* move = mConstraints[minIndex]->mParticleMovement;
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (mConstraints[i]->mParticles[0] == mConstraints[minIndex]->mParticles[0])
				displacements[i] -= move[0] * contactNormals[i];
			else if (mConstraints[i]->mParticles[0] == mConstraints[minIndex]->mParticles[1])
				displacements[i] -= move[1] * contactNormals[i];
			if (mConstraints[i]->mParticles[1])
			{
				if (mConstraints[i]->mParticles[1] == mConstraints[minIndex]->mParticles[0])
					displacements[i] += move[0] * contactNormals[i];
				else if (mConstraints[i]->mParticles[1] == mConstraints[minIndex]->mParticles[1])
					displacements[i] += move[1] * contactNormals[i];
			}
		}
	}

	delete[] displacements;
	delete[] contactNormals;
}

void ParticleManager::applyConstraintsV4(float dt)
{
	const unsigned iterations = mConstraints.size() * mRelaxationConstraints;
	const unsigned numContacts = mConstraints.size();

	for (unsigned iterationsUsed = 0; iterationsUsed < iterations; iterationsUsed++)
	{
		// Find the constraint with the lowest closing velocity or largest seperating velocity if it exists.
		float min = FLT_MAX;
		unsigned minIndex = numContacts;
		for (unsigned i = iterationsUsed % numContacts, i2 = 0; i2 < numContacts; i = (i + 1) % numContacts)
		{
			float cloVel = mConstraints[i]->getRelativeVelocity() * mConstraints[i]->getNormal();
			if (cloVel < min && mConstraints[i]->getDisplacement() > 0)
			{
				min = cloVel;
				minIndex = i;
			}
			i2++;
		}
		// Do we have anything worth resolving?
		if (minIndex == numContacts) 
			return;
		// Apply this constraint.
		mConstraints[minIndex]->applyConstraint(dt);
	}
}

void ParticleManager::startRecord(std::string const & matlab_filename)
{
	if (mRecord)
		stopRecord();
	mRecordName = matlab_filename;
	mBenchmarker.setWindowScreenshotDirectoryName(mRecordName);
	mRecord = true;
}

void ParticleManager::stopRecord(bool createGraph)
{
	if (mRecord)
	{
		if(createGraph)
			mBenchmarker.createGraphs(mRecordName);
		mBenchmarker.reset();
		mRecord = false;
		mBenchmarks = 0;
	}
}

bool ParticleManager::isItRecording(){ return mRecord; }

void ParticleManager::addParticle(Particle* particle)
{
	for (auto plane : mPlanes)
	{
		Constraint* constraint = new ConstraintParticlePlaneA(particle, plane);
		addConstraint(constraint);
		particle->mConstraints.push_back(constraint);
	}
	mParticles.push_back(particle);
	mEntities.push_back(particle);
}

void ParticleManager::addConstraint(Constraint* constraint)
{
	mConstraints.push_back(constraint);
	mEntities.push_back(constraint);
}

void ParticleManager::addCuboid(ParticleCuboid* cuboid)
{
	for (Particle* particle : cuboid->mParticles)
		addParticle(particle);
	for (Constraint* rod : cuboid->mRods)
		addConstraint(rod);
	mCuboids.push_back(cuboid);
	mEntities.push_back(cuboid);
}

void ParticleManager::addRagdoll(Ragdoll<skeleton_type>* ragdoll)
{
	for (ParticleCuboid* cuboid : ragdoll->mCuboids)
		addCuboid(cuboid);
	for (Particle* particle : ragdoll->mParticles)
		addParticle(particle);
	for (Constraint* joint : ragdoll->mJoints)
		addConstraint(joint);
	for (auto pair : ragdoll->mHinges)
	{
		addConstraint(pair.first);
		addConstraint(pair.second);
	}
	mRagdolls.push_back(ragdoll);
	mEntities.push_back(ragdoll);
}

void ParticleManager::addPlane(Plane* plane)
{
	for (auto particle : mParticles)
	{
		Constraint* constraint = new ConstraintParticlePlaneA(particle, plane);
		plane->mConstraints.push_back(constraint);
		mConstraints.push_back(constraint);
	}
	mPlanes.push_back(plane);
	mEntities.push_back(plane);
}


void pc::ParticleManager::removeParticle(Particle* particle)
{
	while (particle->mConstraints.size() > 0)
	{
		Constraint* constraint = (Constraint*)particle->mConstraints[0];
		removeConstraint(constraint);
		eraseElementInVector(constraint, particle->mConstraints);
		delete constraint;
	}
	eraseElementInVector(particle, mParticles);
	eraseElementInVector(particle, mEntities);
}

void pc::ParticleManager::removeConstraint(Constraint* constraint)
{
	eraseElementInVector(constraint, mConstraints);
	eraseElementInVector(constraint, mEntities);
}

void pc::ParticleManager::removeCuboid(ParticleCuboid* cuboid)
{
	for (Particle* particle : cuboid->mParticles)
		removeParticle(particle);
	for (Constraint* rod : cuboid->mRods)
		removeConstraint(rod);
	eraseElementInVector(cuboid, mCuboids);
	eraseElementInVector(cuboid, mEntities);
}

void pc::ParticleManager::removeRagdoll(Ragdoll<skeleton_type>* ragdoll)
{
	for (ParticleCuboid* cuboid : ragdoll->mCuboids)
		removeCuboid(cuboid);
	for (Particle* particle : ragdoll->mParticles)
		removeParticle(particle);
	for (Constraint* joint : ragdoll->mJoints)
		removeConstraint(joint);
	for (auto pair : ragdoll->mHinges)
	{
		removeConstraint(pair.first);
		removeConstraint(pair.second);
	}
	eraseElementInVector(ragdoll, mRagdolls);
	eraseElementInVector(ragdoll, mEntities);
}

void pc::ParticleManager::removePlane(Plane* plane)
{
	for (auto constraint : plane->mConstraints)
	{
		//if (find(plane->mConstraints.begin(), plane->mConstraints.end(), constraint) != plane->mConstraints.end())
		eraseElementInVector(constraint, mConstraints);
	}
	eraseElementInVector(plane, mPlanes);
	eraseElementInVector(plane, mEntities);
}

void ParticleManager::setNextDrawState()
{
	int stateValue = mDrawState;
	mDrawState = (DrawState)((++stateValue) % mDrawStateCount);
}

template<typename element_type_, typename vector_type_>
void ParticleManager::eraseElementInVector(element_type_* element, vector_type_& vector)
{
	auto toErease = std::find(vector.begin(), vector.end(), element);

	// Erase if found.
	if (toErease != vector.end())
		vector.erase(toErease);
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0