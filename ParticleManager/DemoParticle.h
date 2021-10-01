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

using namespace pc;

class DemoParticle : public Demo
{
	Particle* mParticle;
	int mCount = 0;
	bool mToggle = 0;
	float mDT = 1 / 60.f; // Delta time.
	float mDamp = 0.0f;
	float mElasticity = 1.0f;
	Vector mGravity = Vector(0, -10.0f, 0);
	ParticleManager* mParticleManager;
public:
	DemoParticle()
	{
		mParticleManager = new ParticleManager();
		mParticleManager->mElasticity = mElasticity;
		mParticleManager->mGravityAcceleration = mGravity;
		reset();
	}

	void reset()
	{
		mParticleManager->clearAll();

		mParticle = new Particle(Vector(4.5, 5, 0), Vector(), GRAY, 2.0f, mDamp, mDT);
		mParticleManager->addParticle(mParticle);
		mParticleManager->addPlane(new Plane(Vector(5, 3, 0), Vector(0, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(5, 0, 0), Vector(1, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(9, 0, 0), Vector(-1, 1, 0)));
	}

	void update(DemoHandler* draw)
	{
		mParticleManager->update(mDT);

		/// Variables.
		Vector globalPt = mParticle->getPosition();

		/// Inputs.
		// Gives the cuboid an upward force.
		if(draw->keyTyped(' ')) mParticle->addForce(Vector(0, 150, 0) * mParticle->getMass());
		if(draw->keyTyped('f')) mParticle->addForce(Vector(150, 0, 0) * mParticle->getMass());
		// Draw line between anchor point and the mouse position. Adds a spring like force to the cuboid in the direction of the mouse position.
		if(draw->isMouseDown())
		{
			Vector mousePt = Vector(draw->getMouseLocation());
			mParticle->addForce((mousePt - globalPt) * mParticle->getMass());
			draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
		}
		// Switches gravity on/off.
		if(draw->keyTyped('g')) mParticleManager->mIsGravityActive = !mParticleManager->mIsGravityActive;
		// Switches debug on/off.
		if(draw->keyTyped('d')) mParticleManager->setNextDrawState();
		// Resets the simulation.
		if(draw->keyTyped('r')) reset();

		/// Draws.
		mParticleManager->draw(draw);
		if (mParticleManager->mDrawState != mParticleManager->DrawState::DRAW_NORMAL_WITHOUT_GUI)
			drawGUI(draw);
		if (mCount < 2) 
		{
			cout << getString(mParticle->getVelocity()) << ", " << getString(mParticle->getPosition()) << endl;
			if ((mToggle && mParticle->getVelocity().mY < 0) || (!mToggle && 0 < mParticle->getVelocity().mY))
			{
				mToggle = !mToggle;
				mCount++;
			}
		}
	}

	void drawGUI(DemoHandler *draw)
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
		drawNextText(drawPoint, "Elasticity constant: " + getString(mParticleManager->mElasticity), draw);
	}

	void drawNextText(Point &point, string text, DemoHandler *draw) 
	{
		draw->drawText(point, text);
		point.mY -= 0.25;
	}

	const string getName() { return "Particle"; }

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