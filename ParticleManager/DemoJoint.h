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
#include "ConstraintRodB.h"

using namespace pc;

class DemoJoint : public Demo
{
	ParticleCuboid* mCuboid1, *mCuboid2;
	Particle* mParticle;
	unsigned mMode = 1;
	float mDT = 1 / 60.f; // Delta time.
	float mDamp = 0.0f;
	float mElasticity = 1.0f;
	Vector mGravity = Vector(0, -10.0f, 0);
	ParticleManager* mParticleManager;
public:
	DemoJoint()
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
		mCuboid2 = ParticleCuboid::createParticleCuboidV1(Vector(6.5, 6, 0), 1, 1, 1, 2.0f);
		mParticleManager->addCuboid(mCuboid1);
		mParticleManager->addCuboid(mCuboid2);
		pair<Particle*, Particle*> each = mCuboid1->getEachClosestParticle(mCuboid2);
		mParticleManager->addConstraint(new ConstraintRodB(each.first, each.second, 0.0f));
		mParticleManager->addPlane(new Plane(Vector(5, 3, 0), Vector(0, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(5, 0, 0), Vector(1, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(9, 0, 0), Vector(-1, 1, 0)));
	}

	void update(DemoHandler* draw)
	{
		mParticleManager->update(mDT);

		/// Inputs.
		// Gives the cuboid an upward force.
		if (draw->keyTyped(' ')) mCuboid1->addForce(Vector(0, 150, 0) * mCuboid1->getMass());
		if (draw->keyTyped('f')) mCuboid1->addForce(Vector(150, 0, 0) * mCuboid1->getMass());
		// Draw line between anchor point and the mouse position. Adds a spring like force to the cuboid in the direction of the mouse position.
		if (draw->isMouseDown())
		{
			Vector mousePt = Vector(draw->getMouseLocation());
			Vector globalPt;
			switch (mMode)
			{
			case 1:
				globalPt = mCuboid1->getPosition();
				mCuboid1->addForce((mousePt - globalPt) * mCuboid1->getMass());
				draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
				break;
			case 2:
				globalPt = mCuboid2->getPosition();
				mCuboid2->addForce((mousePt - globalPt) * mCuboid2->getMass());
				draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
				break;
			default:
				for (auto particle : mParticleManager->mParticles) 
				{
					//Plane plane = Plane(Vector(), draw->setView());
					//mParticle;
				}
				globalPt = mParticle->getPosition();
				mParticle->addForce((mousePt - globalPt) * mCuboid1->getMass());
				draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
				break;
			}
		}
		// Switches gravity on/off.
		if (draw->keyTyped('g')) mParticleManager->mIsGravityActive = !mParticleManager->mIsGravityActive;
		// Switches debug on/off.
		if (draw->keyTyped('d')) mParticleManager->setNextDrawState();
		// Switches debug on/off.
		if (draw->keyTyped('v')) draw->setView(XY_PLANE);
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

	const string getName() { return "Joint"; }

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