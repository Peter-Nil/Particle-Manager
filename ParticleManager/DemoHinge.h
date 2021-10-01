#pragma once
#include <iomanip>
#include <list>
#include <array>
#include <iterator>
#include <vector>
#include "Vector.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "StringMethods.h"
#include "ParticleCuboid.h"
//#include "ConstraintRodB.h"

using namespace pc;

class DemoHinge : public Demo
{
	static const unsigned cc = 5;
	ParticleCuboid* mCuboids[cc];
	float mDT = 1 / 60.f; // Delta time.
	float mDamp = 0.0f;
	float mElasticity = 0.0f;
	Vector mGravity = Vector(0, -10.0f, 0);
	ParticleManager* mParticleManager;
public:
	DemoHinge()
	{
		mParticleManager = new ParticleManager();
		mParticleManager->mElasticity = mElasticity;
		mParticleManager->mGravityAcceleration = mGravity;
		reset();
	}

	void reset()
	{
		mParticleManager->clearAll();

		mCuboids[0] = ParticleCuboid::createParticleCuboidV1(Vector(4.5, 5, 0), 1, 1, 1, 2.0f);
		mParticleManager->addCuboid(mCuboids[0]);
		for (unsigned i = 1; i < cc; i++)
		{
			mCuboids[i] = ParticleCuboid::createParticleCuboidV1(Vector(4.5 + i, 5 + i, 0), 1, 1, 1, 2.0f);
			mParticleManager->addCuboid(mCuboids[i]);
			pair<Particle*, Particle*> each[2] = { mCuboids[i-1]->getEachClosestParticle(mCuboids[i]), mCuboids[i-1]->getEach2ndClosestParticle(mCuboids[i]) };
			mParticleManager->addConstraint(new ConstraintRodB(each[0].first, each[0].second, 0.0f));
			mParticleManager->addConstraint(new ConstraintRodB(each[1].first, each[1].second, 0.0f));
		}
		mParticleManager->addPlane(new Plane(Vector(5, 3, 0), Vector(0, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(5, 0, 0), Vector(1, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(9, 0, 0), Vector(-1, 1, 0)));
	}

	void update(DemoHandler* draw)
	{
		mParticleManager->update(mDT);

		/// Variables.
		Vector globalPt = mCuboids[0]->getPosition();

		/// Inputs.
		// Gives the cuboid an upward force.
		if (draw->keyTyped(' ')) mCuboids[0]->addForce(Vector(0, 150, 0) * mCuboids[0]->getMass());
		if (draw->keyTyped('f')) mCuboids[0]->addForce(Vector(150, 0, 0) * mCuboids[0]->getMass());
		// Draw line between anchor point and the mouse position. Adds a spring like force to the cuboid in the direction of the mouse position.
		if (draw->isMouseDown())
		{
			Vector mousePt = Vector(draw->getMouseLocation());
			mCuboids[0]->addForce((mousePt - globalPt) * mCuboids[0]->getMass() * cc * 8);
			draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
		}
		// Switches gravity on/off.
		if (draw->keyTyped('g')) mParticleManager->mIsGravityActive = !mParticleManager->mIsGravityActive;
		// Switches debug on/off.
		if (draw->keyTyped('d')) mParticleManager->setNextDrawState();
		// Resets the simulation.
		if (draw->keyTyped('r')) reset();

		/// Draws.
		mParticleManager->draw(draw);
		if (mParticleManager->mDrawState != mParticleManager->DrawState::DRAW_NORMAL_WITHOUT_GUI)
			drawGUI(draw);
	}

	void drawGUI(DemoHandler* draw)
	{
		Point drawPoint = Point(6.5f, 7.f, 0);

		//draw->drawText(drawPoint, "Cuboid velocity: " + PointToString(mCuboid->GetVelocity()));
		//draw->drawText(drawPoint, "Cuboid acceleration: " + PointToString(mCuboid->GetAcceleration()));
		drawNextText(drawPoint, "Delta time: " + to_string(mDT), draw);
		//draw->drawText(drawPoint, "Cuboid angular velocity (L): " + PointToString(mCuboid->GetAngularVelocity()));
		//draw->drawText(drawPoint, "Cuboid mass: " + to_string(mCuboid->GetMass()));
		drawNextText(drawPoint, "Gravity active: " + getString(mParticleManager->mIsGravityActive), draw);
		drawNextText(drawPoint, "Debug active: " + getString(mParticleManager->mDrawState), draw);
		drawNextText(drawPoint, "Gravity constant: " + getString(mParticleManager->mGravityAcceleration), draw);
	}

	void drawNextText(Point& point, string text, DemoHandler* draw)
	{
		draw->drawText(point, text);
		point.mY -= 0.25;
	}

	const string getName() { return "Hinge"; }

	const string getInfo()
	{
		// Info shows only the 5 last rows of text.
		string r = "";
		r += "Press HOME to print starting text\n";
		r += "\n";
		r += "Press g to toggle gravity. Press f to apply force. \n";
		r += "Press d to toggle debug.\n";
		r += "Press r to reset the scenario";
		return r;
	}
};

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0