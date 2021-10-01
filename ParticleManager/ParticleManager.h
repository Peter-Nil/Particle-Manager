#pragma once
#include <vector>
#include "Plane.h"
#include "ConstraintParticlePlaneA.h"
#include "CollisionParticlePlane.h"
#include "Particle.h"
#include "ParticleCuboid.h"
#include "Ragdoll.h"
#include "Benchmarker.h"
#include <assert.h>
#include <queue>

#include <OpenTissue\Kinematics\Skeleton\SkeletonTypes.h>
#include <OpenTissue\Core\Math\MathBasicTypes.h>

namespace pc
{
	class ParticleManager
	{
		typedef OpenTissue::math::default_math_types		math_types;
		typedef OpenTissue::skeleton::Types< math_types >	skeleton_types;
		typedef skeleton_types::skeleton_type				skeleton_type;

		int mDrawStateCount = 5; // Must be equal to amount of states in the DrawState enum.
		vector<Entity*> mEntities;
		vector<Constraint*> mConstraints;
		vector<ParticleCuboid*> mCuboids;
		vector<Ragdoll<skeleton_type>*> mRagdolls;
		vector<Plane*> mPlanes;
		std::priority_queue<int> mPriorityQueue;
		unsigned mConstraintSolverType = 0;
		// For testing purposes.
		Benchmarker mBenchmarker;
		unsigned int mBenchmarks = 0;
		bool mRecord = false;
		std::string mRecordName = "graph";
		float mDT = 1.0f / 60.0f;
		float mDamp = 0.0f;
	public:
		enum DrawState
		{
			DRAW_NORMAL,
			DRAW_NORMAL_WITHOUT_GUI,
			DRAW_DEBUG,
			DRAW_SKIN_ONLY,
			NO_DRAWING
		};

		// How many times constraints shall iterate/repeat.
		unsigned mRelaxationConstraints = 3;
		vector<Particle*> mParticles;
		bool mIsGravityActive = false;
		DrawState mDrawState = DRAW_NORMAL;
		Vector mGravityAcceleration = Vector(0, -9.8f, 0);
		// Elasticity coefficient.
		float mElasticity = 0.5;
		// Constraint solver function pointer. A function pointer which says which constraint solver to use. TODO is it more efficient compared to a switch case?
		void (ParticleManager::* mApplyConstraintsPtr)(float) { &ParticleManager::applyConstraintsV1 };

		ParticleManager();
		~ParticleManager();

		void clearAll();
		// Runs physics, manages collisions, applies constraints, clear forces.
		void update(float dt);
		void runPhysics(float dt);
		void manageCollisions();
		void manageCollisionParticlePlane(Plane* plane, Particle* particle);
		void draw(DemoHandler* draw);
		void setConstraintSolverType(int index);
		std::vector<float>* getStabilityError();

		// Relaxation.
		void applyConstraintsV1(float dt);
		// Relaxation with priority on biggest displacement.
		void applyConstraintsV2(float dt);
		// Relaxation with priority on simulated closing velocity and simulated displacement. Based on Ian constraint solver. TODO.
		void applyConstraintsV3(float dt);
		// Relaxation with priority on largest closing velocity.
		void applyConstraintsV4(float dt);

		void startRecord(std::string const & matlab_filename = "graph");
		void stopRecord(bool createGraph = false);
		bool isItRecording();

		void addParticle(Particle* particle);
		void addConstraint(Constraint* constraint);
		void addCuboid(ParticleCuboid* cuboid);
		void addRagdoll(Ragdoll<skeleton_type>* ragdoll);
		void addPlane(Plane* plane);

		/* Removes the entity from ParticleManager. Does not delete it. Create and use erase if you want to delete entities. */
		// TODO. Are the affected constraints handled correctly?
		void removeParticle(Particle* particle);
		// TODO. Are the affected particles handled correctly?
		void removeConstraint(Constraint* constraint);
		// TODO. Are the affected constraints and particles handled correctly?
		void removeCuboid(ParticleCuboid* cuboid);
		// TODO. Are the affected entities handled correctly?
		void removeRagdoll(Ragdoll<skeleton_type>* ragdoll);
		// TODO. Are the affected constraints handled correctly?
		void removePlane(Plane* plane);

		void setNextDrawState();

		// Calls vector erase if possible.
		template<typename element_type_, typename vector_type_>
		void eraseElementInVector(element_type_ *element, vector_type_ &vector);		
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0