#pragma once
#include "Entity.h"
#include "Vector.h"
#include "Particle.h"
#include "ConstraintRod.h"

namespace pc
{
	class ParticleCuboid : public Entity
	{
		float mMass;
		bool  mDraw = true;
		ParticleCuboid(Vector position = Vector(), float width = 1.0f, float height = 1.0f, float depth = 1.0f, float mass = 1.0f, Color colorFaces = WHITE, Color colorEdges = GRAY);
	public:
		enum ParticlePositions
		{
			LEFT_DOWN_FRONT,
			LEFT_DOWN_BACK,
			LEFT_UP_FRONT,
			LEFT_UP_BACK,
			RIGHT_DOWN_FRONT,
			RIGHT_DOWN_BACK,
			RIGHT_UP_FRONT,
			RIGHT_UP_BACK
		};

		enum Sides
		{
			LEFT,
			RIGHT,
			TOP,
			BOTTOM,
			FRONT,
			BACK
		};

		vector<Particle*> mParticles;
		vector<Constraint*> mRods;
		vector<vector<unsigned>> mFaces;

		virtual ~ParticleCuboid();

		// Creates a cuboid with outer rod constraints and inner diagonal rod constraints. Uses ConstraintRodB.
		static ParticleCuboid* createParticleCuboidV1(Vector position, float width, float height, float depth, float mass, Color colorFaces = WHITE, Color colorEdges = GRAY);
		// Creates a cuboid like version 1 but without inner diagonal rod constraints.
		static ParticleCuboid* createParticleCuboidV2(Vector position, float width, float height, float depth, float mass, Color colorFaces = WHITE, Color colorEdges = GRAY);
		// Creates a cuboid like version 1 but uses ConstraintRodA.
		static ParticleCuboid* createParticleCuboidV3(Vector position, float width, float height, float depth, float mass, Color colorFaces = WHITE, Color colorEdges = GRAY);
		// Creates a cuboid like version 2 but uses ConstraintRodA.
		static ParticleCuboid* createParticleCuboidV4(Vector position, float width, float height, float depth, float mass, Color colorFaces = WHITE, Color colorEdges = GRAY);

		ConstraintRod* createJoint(ParticleCuboid* cuboid);
		pair<ConstraintRod*, ConstraintRod*> createHinge(ParticleCuboid* cuboid);

		virtual void draw(DemoHandler* draw, const bool drawDebug = false);
		void setDrawActive(bool value);
		void addForce(Vector f);

		Vector getPosition() const;
		float getMass() const;
		pair<Particle*, Particle*> getEachClosestParticle(ParticleCuboid* cuboid);
		pair<Particle*, Particle*> getEach2ndClosestParticle(ParticleCuboid* cuboid);
		// Returns a particle pair from which the center point of the selected cuboid side can be retrieved.
		pair<Particle*, Particle*> getSideDiagonalParticles(Sides side);
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0