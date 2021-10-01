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

using namespace pc;

class DemoParticleCuboid : public Demo
{
	ParticleCuboid /**mTarget,*/ *mCuboid1, *mCuboid2;
	vector<ParticleCuboid*> mTargets;
	float mDT = 1 / 60.f; // Delta time.
	float mDamp = 0.0f;
	float mElasticity = 1.0f;
	Vector mGravity = Vector(0, -10.0f, 0);
	ParticleManager* mParticleManager;
	Benchmarker mBenchmarker;
	int mTimeChecks = 0;
public:
	DemoParticleCuboid()
	{
		mParticleManager = new ParticleManager();
		mParticleManager->mElasticity = mElasticity;
		mParticleManager->mGravityAcceleration = mGravity;
		reset();
	}

	void reset()
	{
		mParticleManager->clearAll();
		
		mCuboid1 = ParticleCuboid::createParticleCuboidV1(Vector(4.5, 5, 0), 1, 1, 1, 2.0f);
		mCuboid2 = ParticleCuboid::createParticleCuboidV3(Vector(9.5, 5, 0), 1, 1, 1, 2.0f);
		mParticleManager->addCuboid(mCuboid1);
		mParticleManager->addCuboid(mCuboid2);
		mParticleManager->addPlane(new Plane(Vector(5, 3, 0), Vector( 0, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(5, 0, 0), Vector( 1, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(9, 0, 0), Vector(-1, 1, 0)));
		mTargets.clear();
		mTargets.push_back(mCuboid1);
	}

	void update(DemoHandler* draw)
	{
		// TODO: maybe it should instead use the benchmarker in particleManager?
		//mBenchmarker.setStartTimeToNow();
		mParticleManager->update(mDT);
		/*mBenchmarker.storeDuration();
		mTimeChecks++;

		if (mTimeChecks >= 100)
		{
			mBenchmarker.printResult<chrono::microseconds>();
			mTimeChecks = 0;
		}

		mBenchmarker.printDelaTime<chrono::milliseconds>();*/

		/// Inputs.
		// Gives the cuboid an upward force.
		if (draw->keyTyped(' ')) 
			for (auto target : mTargets)
				target->addForce(Vector(0, 150, 0) * target->getMass());
		if (draw->keyTyped('f'))
			for (auto target : mTargets)
				target->addForce(Vector(150, 0, 0) * target->getMass());
		// Draw line between anchor point and the mouse position. Adds a spring like force to the cuboid in the direction of the mouse position.
		if (draw->isMouseDown())
		{
			for (auto target : mTargets)
			{
				Vector globalPt = target->getPosition();
				Vector mousePt = Vector(draw->getMouseLocation());
				target->addForce((mousePt - globalPt) * target->getMass());
				draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
			}
		}
		// Switches gravity on/off.
		if (draw->keyTyped('g')) mParticleManager->mIsGravityActive = !mParticleManager->mIsGravityActive;
		// Switches debug on/off.
		if (draw->keyTyped('d')) mParticleManager->setNextDrawState();
		// Change target.
		if (draw->keyTyped('1'))
		{
			mTargets.clear();
			mTargets.push_back(mCuboid1);
		}
		if (draw->keyTyped('2'))
		{
			mTargets.clear();
			mTargets.push_back(mCuboid2);
		}
		if (draw->keyTyped('3'))
		{
			mTargets.clear();
			mTargets.push_back(mCuboid1);
			mTargets.push_back(mCuboid2);
		}
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

	const string getName() { return "Particle Cuboid"; }

	const string getInfo()
	{
		// Info shows only the 5 last rows of text.
		string r = "";
		r += "Press HOME to print starting text\n";
		r += "\n";
		r += "Press 1, 3 or 2 to change mouse target(s). Press g to toggle gravity.\n";
		r += "Mouse click or Press f or space to apply force. Press d to toggle debug.\n";
		r += "Press r to reset the scenario";
		return r;
	}
};

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0