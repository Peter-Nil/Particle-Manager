#include "ParticleCuboid.h"
#include "ConstraintRodA.h"
#include "ConstraintRodB.h"
using namespace pc;

ParticleCuboid::ParticleCuboid(Vector position, float width, float height, float depth, float mass, Color colorFaces, Color colorEdges)
{
	mMass = mass;
	mColor = colorFaces;
	float dividedMass = mass / 8.0f;
	for (float fx : {-0.5f, 0.5f})
		for (float fy : {-0.5f, 0.5f})
			for (float fz : {-0.5f, 0.5f})
				mParticles.push_back(new Particle(position + Vector(fx * width, fy * height, fz * depth), Vector(), colorEdges, dividedMass));

	// TODO flat 4p polygon instead of triangles.
	mFaces.push_back({0, 3, 1}); // Sida 0.
	mFaces.push_back({0, 2, 3}); // Sida 1.
	mFaces.push_back({0, 1, 5}); // Sida 2.
	mFaces.push_back({0, 5, 4}); // Sida 3.
	mFaces.push_back({0, 6, 2}); // Sida 4.
	mFaces.push_back({0, 4, 6}); // Sida 5.
	mFaces.push_back({7, 4, 5}); // Sida 6.
	mFaces.push_back({7, 6, 4}); // Sida 7.
	mFaces.push_back({7, 3, 2}); // Sida 8.
	mFaces.push_back({7, 2, 6}); // Sida 9.
	mFaces.push_back({7, 1, 3}); // Sida 10.
	mFaces.push_back({7, 5, 1}); // Sida 11.
}


pc::ParticleCuboid::~ParticleCuboid()
{
	mParticles.clear();
	mFaces.clear();
}

ParticleCuboid* ParticleCuboid::createParticleCuboidV1(Vector position, float width, float height, float depth, float mass, Color colorFaces, Color colorEdges)
{
	ParticleCuboid* cuboid = new ParticleCuboid(position, width, height, depth, mass, colorFaces, colorEdges);

	for (unsigned i = 0; i < cuboid->mParticles.size(); ++i)
		for (unsigned j = i + 1; j < cuboid->mParticles.size(); ++j)
			cuboid->mRods.push_back(new ConstraintRodB(cuboid->mParticles[i], cuboid->mParticles[j], colorEdges));

	return cuboid;
}

ParticleCuboid* ParticleCuboid::createParticleCuboidV2(Vector position, float width, float height, float depth, float mass, Color colorFaces, Color colorEdges)
{
	ParticleCuboid* cuboid = new ParticleCuboid(position, width, height, depth, mass, colorFaces, colorEdges);

	for (unsigned i = 0; i < cuboid->mParticles.size(); ++i)
		for (unsigned j = i + 1; j < cuboid->mParticles.size(); ++j)
		{
			int count = 0;
			if (cuboid->mParticles[i]->getPosition().mX == cuboid->mParticles[j]->getPosition().mX) count++;
			if (cuboid->mParticles[i]->getPosition().mY == cuboid->mParticles[j]->getPosition().mY) count++;
			if (cuboid->mParticles[i]->getPosition().mZ == cuboid->mParticles[j]->getPosition().mZ) count++;

			if (count != 0)
				cuboid->mRods.push_back(new ConstraintRodB(cuboid->mParticles[i], cuboid->mParticles[j], colorEdges));
		}

	return cuboid;
}

ParticleCuboid* ParticleCuboid::createParticleCuboidV3(Vector position, float width, float height, float depth, float mass, Color colorFaces, Color colorEdges)
{
	ParticleCuboid* cuboid = new ParticleCuboid(position, width, height, depth, mass, colorFaces, colorEdges);

	for (unsigned i = 0; i < cuboid->mParticles.size(); ++i)
		for (unsigned j = i + 1; j < cuboid->mParticles.size(); ++j)
			cuboid->mRods.push_back(new ConstraintRodA(cuboid->mParticles[i], cuboid->mParticles[j], colorEdges));

	return cuboid;
}

ParticleCuboid* ParticleCuboid::createParticleCuboidV4(Vector position, float width, float height, float depth, float mass, Color colorFaces, Color colorEdges)
{
	ParticleCuboid* cuboid = new ParticleCuboid(position, width, height, depth, mass, colorFaces, colorEdges);

	for (unsigned i = 0; i < cuboid->mParticles.size(); ++i)
		for (unsigned j = i + 1; j < cuboid->mParticles.size(); ++j)
		{
			int count = 0;
			if (cuboid->mParticles[i]->getPosition().mX == cuboid->mParticles[j]->getPosition().mX) count++;
			if (cuboid->mParticles[i]->getPosition().mY == cuboid->mParticles[j]->getPosition().mY) count++;
			if (cuboid->mParticles[i]->getPosition().mZ == cuboid->mParticles[j]->getPosition().mZ) count++;

			if (count != 0)
				cuboid->mRods.push_back(new ConstraintRodA(cuboid->mParticles[i], cuboid->mParticles[j], colorEdges));
		}

	return cuboid;
}

ConstraintRod* ParticleCuboid::createJoint(ParticleCuboid* cuboid)
{
	pair<Particle*, Particle*> each = getEachClosestParticle(cuboid);
	// Color particles connected to the joint
	Color color = CYAN;
	if (each.first->getColor()  == PINK || each.first->getColor()  == GREEN ||
		each.second->getColor() == PINK || each.second->getColor() == GREEN)
		color = GREEN;
	each.first->setColor(color);
	each.second->setColor(color);
	// TODO. Make this more efficient somehow. Maybe use templates instead?
	ConstraintRod* ret; 
	if (dynamic_cast<ConstraintRodA*>(mRods[0]))
		ret = new ConstraintRodA(each.first, each.second, 0.0f, color);
	else if (dynamic_cast<ConstraintRodB*>(mRods[0]))
		ret = new ConstraintRodB(each.first, each.second, 0.0f, color);
	else
	{
		assert(false && "particleCuboid: constraintRod type missing");
		ret = nullptr;
	}
	return ret;
}

pair<ConstraintRod*, ConstraintRod*> ParticleCuboid::createHinge(ParticleCuboid* cuboid)
{
	pair<Particle*, Particle*> each[2] = { getEachClosestParticle(cuboid), getEach2ndClosestParticle(cuboid) };

	// Color edges and particles connected to the hinge
	Color color = PINK;
	for (auto constraint : mRods)
	{
		Particle* particleFound;
		if (((constraint->mParticles[0] == (particleFound = each[0].first) || constraint->mParticles[1] == (particleFound = each[0].first)) &&
			(constraint->mParticles[0] == (particleFound = each[1].first) || constraint->mParticles[1] == (particleFound = each[1].first))))
		{
			for (auto constraintOther : cuboid->mRods)
			{
				if ((constraintOther->mParticles[0] == each[0].second || constraintOther->mParticles[1] == each[0].second) &&
					(constraintOther->mParticles[0] == each[1].second || constraintOther->mParticles[1] == each[1].second))
				{
					color = PINK;
					if (constraint->mParticles[0]->getColor() == CYAN || constraint->mParticles[0]->getColor() == GREEN ||
						constraint->mParticles[1]->getColor() == CYAN || constraint->mParticles[1]->getColor() == GREEN ||
						constraintOther->mParticles[0]->getColor() == CYAN || constraintOther->mParticles[0]->getColor() == GREEN ||
						constraintOther->mParticles[1]->getColor() == CYAN || constraintOther->mParticles[0]->getColor() == GREEN)
						color = GREEN;
					constraint->mParticles[0]->setColor(color);
					constraint->mParticles[1]->setColor(color);
					constraintOther->mParticles[0]->setColor(color);
					constraintOther->mParticles[1]->setColor(color);
					constraint->setColor(PINK);
					constraintOther->setColor(PINK);
					break;
				}
			}
			break;
		}
	}

	float len = (each[1].second->getPosition() - each[1].first->getPosition()).length() - (each[0].second->getPosition() - each[0].first->getPosition()).length();
	// TODO. Make this more efficient somehow. Maybe use templates instead?
	pair<ConstraintRod*, ConstraintRod*> ret;
	if (dynamic_cast<ConstraintRodA*>(mRods[0]))
		ret = pair<ConstraintRodA*, ConstraintRodA*>(new ConstraintRodA(each[0].first, each[0].second, 0.0f, color), new ConstraintRodA(each[1].first, each[1].second, len, color));
	else if (dynamic_cast<ConstraintRodB*>(mRods[0]))
		ret = pair<ConstraintRodB*, ConstraintRodB*>(new ConstraintRodB(each[0].first, each[0].second, 0.0f, color), new ConstraintRodB(each[1].first, each[1].second, len, color));
	else
	{
		assert(false && "particleCuboid: constraintRod type missing");
		ret = pair<ConstraintRod*, ConstraintRod*>(nullptr, nullptr);
	}
	return ret;
}


void ParticleCuboid::draw(DemoHandler* draw, const bool drawDebug)
{
	if (mDraw)
	{
		if (!drawDebug)
			for (auto index : mFaces)
				draw->drawPolygon({ mParticles[index[0]]->getPosition().getPoint(), mParticles[index[2]]->getPosition().getPoint(), mParticles[index[1]]->getPosition().getPoint() }, mColor);
		else
			for (auto index : mFaces)
				draw->drawPolygon({ mParticles[index[0]]->getPosition().getPoint(), mParticles[index[1]]->getPosition().getPoint(), mParticles[index[2]]->getPosition().getPoint() }, mColor);
	}
}

void pc::ParticleCuboid::setDrawActive(bool value)
{
	mDraw = value;
}

void ParticleCuboid::addForce(Vector f)
{
	Vector dividedForce = f * 0.25f;
	for (Particle* particle : mParticles)
		particle->addForce(dividedForce);
}


Vector ParticleCuboid::getPosition() const
{
	Vector res = Vector();
	for (Particle* particle : mParticles)
		res += particle->getPosition() * 0.125f;
	return res;
}

float ParticleCuboid::getMass() const
{
	return mMass;
}

pair<Particle*, Particle*> ParticleCuboid::getEachClosestParticle(ParticleCuboid* cuboid)
{
	pair<Particle*, Particle*> each = { mParticles[0], cuboid->mParticles[0] };
	float currentLen = (mParticles[0]->getPosition() - cuboid->mParticles[0]->getPosition()).sqrLength();
	for(Particle* particle1 : mParticles)
		for (Particle* particle2 : cuboid->mParticles)
		{
			float testLen = (particle2->getPosition() - particle1->getPosition()).sqrLength();
			if (testLen < currentLen)
			{
				currentLen = testLen;
				each.first = particle1;
				each.second = particle2;
			}
		}
	return each;
}

pair<Particle*, Particle*> ParticleCuboid::getEach2ndClosestParticle(ParticleCuboid* cuboid)
{
	pair<Particle*, Particle*> each1st = { mParticles[0], cuboid->mParticles[0] };
	pair<Particle*, Particle*> each2nd = { mParticles[0], cuboid->mParticles[0] };
	float currentLen1 = (mParticles[0]->getPosition() - cuboid->mParticles[0]->getPosition()).sqrLength();
	float currentLen2 = currentLen1;
	for (Particle* particle1 : mParticles)
		for (Particle* particle2 : cuboid->mParticles)
		{
			float testLen = (particle2->getPosition() - particle1->getPosition()).sqrLength();
			if (testLen < currentLen1)
			{
				currentLen1 = testLen;
				each2nd.first = each1st.first;
				each2nd.second = each1st.second;
				each1st.first = particle1;
				each1st.second = particle2;
			} else if (testLen < currentLen2)
			{
				currentLen2 = testLen;
				each2nd.first = particle1;
				each2nd.second = particle2;
			}
		}
	return each2nd;
}

pair<Particle*, Particle*> ParticleCuboid::getSideDiagonalParticles(Sides side)
{
	switch (side)
	{
	case Sides::LEFT:
		return std::make_pair(mParticles[ParticlePositions::LEFT_UP_BACK], mParticles[ParticlePositions::LEFT_DOWN_FRONT]);
	case Sides::RIGHT:
		return std::make_pair(mParticles[ParticlePositions::RIGHT_UP_FRONT], mParticles[ParticlePositions::RIGHT_DOWN_BACK]);
	case Sides::TOP:
		return std::make_pair(mParticles[ParticlePositions::LEFT_UP_BACK], mParticles[ParticlePositions::RIGHT_UP_FRONT]);
	case Sides::BOTTOM:
		return std::make_pair(mParticles[ParticlePositions::RIGHT_DOWN_FRONT], mParticles[ParticlePositions::LEFT_DOWN_BACK]);
	case Sides::FRONT:
		return std::make_pair(mParticles[ParticlePositions::LEFT_UP_FRONT], mParticles[ParticlePositions::RIGHT_DOWN_FRONT]);
	case Sides::BACK:
		return std::make_pair(mParticles[ParticlePositions::RIGHT_UP_BACK], mParticles[ParticlePositions::LEFT_DOWN_BACK]);
	default:
		return pair<Particle*, Particle*>(nullptr, nullptr);
	}
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0