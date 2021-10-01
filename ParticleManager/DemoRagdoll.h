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
#include "Ragdoll.h"

#include <OpenTissue\Kinematics\Skeleton\IO\SkeletonXMLRead.h>
#include <OpenTissue\Kinematics\Skeleton\IO\SkeletonCal3DXMLRead.h>

#include <OpenTissue\Kinematics\animation\IO\AnimationKeyframeAnimationXMLRead.h>
#include <OpenTissue\Kinematics\animation\IO\AnimationKeyframeAnimationCal3dXMLRead.h>

#include <OpenTissue\Kinematics\Skinning\IO\SkinningXMLRead.h>
#include <OpenTissue\Kinematics\Skinning\IO\SkinningCal3dXMLRead.h>
#include <OpenTissue\Kinematics\Skinning\IO\SkinningMaterialXMLRead.h>
#include <OpenTissue\Kinematics\Skinning\IO\SkinningMaterialCal3DXMLRead.h>

#include <OpenTissue\Kinematics\Skeleton\SkeletonTypes.h>
#include <OpenTissue\Kinematics\Animation\AnimationNaiveBlendScheduler.h>
#include <OpenTissue\Kinematics\Animation\AnimationKeyframeAnimation.h>

#include <OpenTissue\Kinematics\Skinning\SkinManipulation.h>
#include <OpenTissue\Core\Math\MathCoordsys.h>

using namespace pc;

class DemoRagdoll : public Demo
{	
	float						mDT						= 1 / 60.f; // Delta time.
	float						mDamp					= 0.0f;
	float						mElasticity				= 1.0f;
	BodyPartIndex				mSelectedBodyPartIndex	= BodyPartIndex::LEFT_FOREARM;
	Vector						mGravity				= Vector(0, -10.0f, 0);
	ParticleManager*			mParticleManager;
	unsigned					mRagdollType			= 1,
								mSolverType				= 1;
	Plane*						mTestPlane				= new Plane(Vector(0, 5.0f, 0), Vector(0, 1.0f, 0));

	double						mCurrentXRad			= 0;
	double						mCurrentYRad			= 0;
	double						mCurrentZRad			= 0;

	bool						mShowJointPositions		= false;

public:
#pragma region OpenTissue
	/// Types used for skeleton and skinning from OpenTissue.
	bool b[256]; // Boolean array used to keep track of the user actions/selections.
	typedef float																	real_type;
	typedef OpenTissue::math::default_math_types									math_types;
	typedef math_types::matrix3x3_type												matrix3x3_type;
	typedef math_types::quaternion_type												quaternion_type;
	typedef math_types::vector3_type												vector3_type;
	typedef OpenTissue::skeleton::Types< math_types >								skeleton_types;
	typedef skeleton_types::skeleton_type											skeleton_type;
	typedef skeleton_types::bone_type												bone_type;
	typedef Ragdoll<skeleton_type>::skin_type										skin_type;
	typedef Ragdoll<skeleton_type>::skin_render_type								skin_render_type;

	typedef OpenTissue::animation::KeyframeAnimation<skeleton_type>					keyframe_animation_type;
	typedef OpenTissue::animation::NaiveBlendScheduler<skeleton_type>				naive_blend_scheduler_type;

	keyframe_animation_type     m_animation[100];				// The raw animations.
	naive_blend_scheduler_type  m_blend_scheduler;				// The animation blend scheduler (combines raw animations into final animation).
	real_type                   m_time;							// The current animation time.
	real_type                   m_delta_time;					// Time inbetween two poses (i.e. 1/fps).
	real_type                   m_duration;						// Duration of animation.
	skin_type					m_skin;							// Skin container.
	bool                        m_display_bones;				// Boolean flag used to turn on/off rendering of bones.
#pragma endregion

	// Some key values used for user-interface.
	static unsigned char const TWO_KEY    = '2';
	static unsigned char const THREE_KEY  = '3';
	static unsigned char const FOUR_KEY   = '4';
	static unsigned char const FIVE_KEY   = '5';
	static unsigned char const SIX_KEY    = '6';
	static unsigned char const SEVEN_KEY  = '7';
	static unsigned char const EIGHT_KEY  = '8';

private:
	Ragdoll<skeleton_type>* mRagdoll;

public:
	DemoRagdoll()
	{
		mParticleManager						= new ParticleManager();
		mParticleManager->mElasticity			= mElasticity;
		mParticleManager->mGravityAcceleration	= mGravity;
		mRagdoll								= Ragdoll<skeleton_type>::createRagdollV3(Vector(4.5, 5, 0));
		reset();
	}

	void reset()
	{
		// Clear and delete old data.
		mParticleManager->clearAll();

		// Create one of the predefined ragdoll types.
		switch (mRagdollType)
		{
		case(1): mRagdoll = Ragdoll<skeleton_type>::createRagdollV1(Vector(4.5, 5, 0));
			break;
		case(2): mRagdoll = Ragdoll<skeleton_type>::createRagdollV2(Vector(4.5, 5, 0));
			break;
		default: mRagdoll = Ragdoll<skeleton_type>::createRagdollV3(Vector(4.5, 5, 0));
			break;
		}

		initSkinning();
		mRagdoll->initBones();

		// Add the created ragdoll and some planes to the particle manager.
		mParticleManager->addRagdoll(mRagdoll);
		mParticleManager->addPlane(new Plane(Vector(5, 3, 0), Vector( 0, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(5, 0, 0), Vector( 1, 1, 0)));
		mParticleManager->addPlane(new Plane(Vector(9, 0, 0), Vector(-1, 1, 0)));
	}

	void update(DemoHandler* draw)
	{
		mParticleManager->update(mDT);

		/// Variables.
		Vector globalPt = mRagdoll->getPosition(mSelectedBodyPartIndex);

		/// Inputs.
		// Draw line between anchor point and the mouse position. Adds a spring like force to the ragdoll in the direction of the mouse position.
		if (draw->isMouseDown())
		{
			Vector mousePt = Vector(draw->getMouseLocation());
			mRagdoll->addForce(mSelectedBodyPartIndex, (mousePt - globalPt) * mRagdoll->getMass());
			draw->drawLine(globalPt.getPoint(), mousePt.getPoint());
		}
		
		// Set ragdoll type to 1. 
		if (draw->keyTyped('1'))
		{
			mRagdollType = 1;
			reset();
		}
		// Set ragdoll type to 2.
		if (draw->keyTyped('2'))
		{
			mRagdollType = 2;
			reset();
		}
		// Set ragdoll type to 3.
		if (draw->keyTyped('3'))
		{
			mRagdollType = 3;
			reset();
		}
		// Set constraint solver type to 1: uses relaxation. 3 relaxation loops.
		if (draw->keyTyped('4'))
		{
			mSolverType = 1;
			mParticleManager->setConstraintSolverType(1);
			mParticleManager->mRelaxationConstraints = 3;
		}
		// Set constraint solver type to 2: uses relaxation and priority on biggest displacement. 3 relaxation loops.
		if (draw->keyTyped('5'))
		{
			mSolverType = 2;
			mParticleManager->setConstraintSolverType(2);
			mParticleManager->mRelaxationConstraints = 3;
		}
		// Set constraint solver type to 2: uses relaxation and priority on biggest displacement. 1 relaxation loops.
		if (draw->keyTyped('6'))
		{
			mSolverType = 3;
			mParticleManager->setConstraintSolverType(2);
			mParticleManager->mRelaxationConstraints = 1;
		}
		// Set constraint solver type to 3: uses relaxation and priority on simulated closing velocity and simulated displacement. 3 relaxation loops. TODO
		if (draw->keyTyped('7'))
		{
			mSolverType = 4;
			mParticleManager->setConstraintSolverType(3);
			mParticleManager->mRelaxationConstraints = 3;
		}
		// Set constraint solver type to 4: uses relaxation and priority on largest closing velocity. 3 relaxation loops.
		if (draw->keyTyped('8'))
		{
			mSolverType = 5;
			mParticleManager->setConstraintSolverType(4);
			mParticleManager->mRelaxationConstraints = 3;
		}

		// Creates a ragdoll. TODO.
		//if (draw->keyTyped('w'))
		//	mParticleManager->addRagdoll(mRagdoll);
		// Removes the ragdoll. TODO.
		//if (draw->keyTyped('e'))
		//	mParticleManager->removeRagdoll(mRagdoll);
		// Resets the simulation.
		if (draw->keyTyped('r'))
			reset();
		// Starts the recording.
		if (draw->keyTyped('t'))
			mParticleManager->startRecord("graph" + getString(mRagdollType) + "x" + getString(mSolverType));
		//if (draw->keyTyped('y')) mParticleManager->stopRecord();
		// Stops the recording.
		if (draw->keyTyped('y')) 
			mParticleManager->stopRecord();
		// Starts scenario 1. 
		if (draw->keyTyped('u'))
		{
			reset();
			mParticleManager->startRecord("graph" + getString(mRagdollType) + "x" + getString(mSolverType) + "_Senario1");
		}
		// Starts scenario 2. 
		if (draw->keyTyped('i'))
		{
			reset();
			mParticleManager->startRecord("graph" + getString(mRagdollType) + "x" + getString(mSolverType) + "_Senario2");
			mRagdoll->addForce(RIGHT_FOREARM, Vector(150, 0, 0) * mRagdoll->getMass());
		}
		// Replaces current ragdoll with ragdoll v1 without resetting demo. TODO.
		//if (draw->keyTyped('o'))
		//{
		//	mParticleManager->eraseRagdoll(mRagdoll);
		//	mRagdoll = Ragdoll<skeleton_type>::createRagdollV1(Vector(4.5, 5, 0));
		//	mParticleManager->addRagdoll(mRagdoll);
		//
		//	initSkinning();
		//	mRagdoll->initBones();
		//}
			
		// Gives the selected body part an upward force push.
		if (draw->keyTyped('a'))
			mRagdoll->addForce(mSelectedBodyPartIndex, Vector(0, 150, 0) * mRagdoll->getMass());
		// Gives the selected body part an eastward force push.
		if (draw->keyTyped('s'))
			mRagdoll->addForce(mSelectedBodyPartIndex, Vector(150, 0, 0) * mRagdoll->getMass());
		// Switches draw state e.g. debug mode.
		if (draw->keyTyped('d'))
			mParticleManager->setNextDrawState();
		// Switches skin rendering on/off.
		if (draw->keyTyped('f'))
			mRagdoll->toggleRenderSkin();
		// Switches gravity on/off.
		if (draw->keyTyped('g'))
			mParticleManager->mIsGravityActive = !mParticleManager->mIsGravityActive;
		// Decrements/Increments selected body part index.
		if (draw->keyTyped('j') || draw->keyTyped('k'))
		{
			draw->keyTyped('j') ? mSelectedBodyPartIndex = (BodyPartIndex)(mSelectedBodyPartIndex - 1) : (BodyPartIndex)(mSelectedBodyPartIndex + 1);
			// Keep index within boundaries.
			mSelectedBodyPartIndex = (BodyPartIndex)((mRagdoll->mBodyParts.size() + mSelectedBodyPartIndex % mRagdoll->mBodyParts.size()) % mRagdoll->mBodyParts.size());
		}
		// Switches ragdoll joint position rendering on/off
		if (draw->keyTyped('l'))
			mShowJointPositions = !mShowJointPositions;
		
		// For testing only: rotates ragdoll skin along z-axel.
		if (draw->keyTyped('z'))
		{
			mCurrentZRad = fmod(mCurrentZRad + M_1_PI, M_PI * 2);

			typename skeleton_type::bone_iterator boneIterator = mRagdoll->begin();

			for (; boneIterator != mRagdoll->end(); ++boneIterator)
			{
				quaternion_type result;
				result.rotate(mCurrentXRad, mCurrentYRad, mCurrentZRad);
				boneIterator->bone_space_transform().Q() = result;
			}
		}
		// For testing only: rotates ragdoll skin along x-axel.
		if (draw->keyTyped('x'))
		{
			mCurrentXRad = fmod(mCurrentXRad + M_1_PI, M_PI * 2);

			//typename skeleton_type::bone_iterator boneIterator = mRagdoll->begin();

			//for (; boneIterator != mRagdoll->end(); ++boneIterator)
			//{
			//	quaternion_type result;
			//	result.rotate(mCurrentXRad, mCurrentYRad, mCurrentZRad);
			//	boneIterator->bone_space_transform().Q() = result;
			//	//boneIterator->bone_space_transform().T() -= boneIterator->bone_space_transform().Q().rotate(boneIterator->absolute().T());
			//}

			/*quaternion_type result;
			result.rotate(mCurrentXRad, mCurrentYRad, mCurrentZRad);*/

			bone_type* bone = mRagdoll->get_bone("Cally L Hand");

			bone->bone_space_transform().Q().lookAt(vector3_type(0, 0, 0), vector3_type(0, 1, 0),
													vector3_type(0, 0, 1), vector3_type(0, 1, 0));

			bone_type* bone2 = mRagdoll->get_bone("Cally L Forearm");

			bone2->bone_space_transform().Q().lookAt(vector3_type(0, 0, 0), vector3_type(0, 1, 0),
													 vector3_type(0, 0, 1), vector3_type(0, 1, 0));

			bone_type* bone3 = mRagdoll->get_bone("Cally R Hand");

			bone3->bone_space_transform().Q().lookAt(vector3_type(0, 0, 0), vector3_type(0, 1, 0),
													 vector3_type(0, 0, 1), vector3_type(0, 1, 0));

			bone_type* bone4 = mRagdoll->get_bone("Cally R Forearm");

			bone4->bone_space_transform().Q().lookAt(vector3_type(0, 0, 0), vector3_type(0, 1, 0),
													 vector3_type(0, 0, 1), vector3_type(0, 1, 0));
		}
		// For testing only: rotates ragdoll skin along y-axel.
		if (draw->keyTyped('c'))
		{
			mCurrentYRad = fmod(mCurrentYRad + M_1_PI, M_PI * 2);

			typename skeleton_type::bone_iterator boneIterator = mRagdoll->begin();

			for (; boneIterator != mRagdoll->end(); ++boneIterator)
			{
				quaternion_type result;
				result.rotate(mCurrentXRad, mCurrentYRad, mCurrentZRad);
				boneIterator->bone_space_transform().Q() = result;
			}
		}
		// Perform raycast TODO.
		//if (draw->keyTyped('v'))
		//{
		//	Raycast2 ray = Raycast2();
		//	cout << ray.performRaycast(mRagdoll->mBodyParts, Vector(3.0f, 3.0f, 1.0f));
		//}
		// Prints info about the bones of the ragdoll.
		if (draw->keyTyped('b'))
			mRagdoll->printBoneInfo();

		/// Draws.
		mParticleManager->draw(draw);
		if (mShowJointPositions)
			mRagdoll->drawBoneJointConnections(draw);
		if (mParticleManager->mDrawState != mParticleManager->DrawState::DRAW_NORMAL_WITHOUT_GUI)
			drawGUI(draw);
	}

	void initSkinning()
	{
		//this->camera().move( -400 );

		m_blend_scheduler.clear();
		m_duration      = 0.0; 
		b[TWO_KEY]      = false;
		b[THREE_KEY]    = false;
		b[FOUR_KEY]     = false;
		b[FIVE_KEY]     = false;
		b[SIX_KEY]      = false;
		b[SEVEN_KEY]    = false;
		b[EIGHT_KEY]    = false;

		//std::string data_path = opentissue_path;

		OpenTissue::skeleton::xml_read("ModelData/XML/character1.xml", *mRagdoll);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_idle.xml",           m_animation[0]);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_jog.xml",            m_animation[1]);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_shoot_arrow.xml",    m_animation[2]);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_strut.xml",          m_animation[3]);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_tornado_kick.xml",   m_animation[4]);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_walk.xml",           m_animation[5]);
		OpenTissue::animation::keyframe_animation_xml_read("ModelData/XML/character1_wave.xml",           m_animation[6]);

		//--- ideally the weights should be controlled by the scheduler,
		//--- but for demo purpose we just set the weights here!!!
		m_animation[0].set_weight(1.0);
		m_animation[1].set_weight(1.0);
		m_animation[2].set_weight(5.0);
		m_animation[3].set_weight(1.0);
		m_animation[4].set_weight(1.0);
		m_animation[5].set_weight(1.0);
		m_animation[6].set_weight(5.0);

		// Clear skin parts and set materials to default.
		for(size_t i=0u; i<m_skin.m_sz; ++i)
		{
			m_skin.m_skin_parts[i].clear();
			m_skin.m_material[i].set_default();
		}

		// Read in new skin parts data.
		OpenTissue::skinning::xml_read("ModelData/XML/character1_calf_left.xml",      m_skin.m_skin_parts[0 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_hand_right.xml",     m_skin.m_skin_parts[1 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_ponytail.xml",       m_skin.m_skin_parts[2 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_calf_right.xml",     m_skin.m_skin_parts[3 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_head.xml",           m_skin.m_skin_parts[4 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_thigh_left.xml",     m_skin.m_skin_parts[5 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_chest.xml",          m_skin.m_skin_parts[6 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_lowerarm_left.xml",  m_skin.m_skin_parts[7 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_thigh_right.xml",    m_skin.m_skin_parts[8 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_foot_left.xml",      m_skin.m_skin_parts[9 ]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_lowerarm_right.xml", m_skin.m_skin_parts[10]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_upperarm_left.xml",  m_skin.m_skin_parts[11]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_foot_right.xml",     m_skin.m_skin_parts[12]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_neck.xml",           m_skin.m_skin_parts[13]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_upperarm_right.xml", m_skin.m_skin_parts[14]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_hand_left.xml",      m_skin.m_skin_parts[15]);
		OpenTissue::skinning::xml_read("ModelData/XML/character1_pelvis.xml",         m_skin.m_skin_parts[16]);

		// Scale skeleton and skin to fit the ragdoll.
		typename skeleton_type::bone_iterator boneIterator = mRagdoll->begin();

		for (; boneIterator != mRagdoll->end(); ++boneIterator)
		{
			boneIterator->relative().T()	*= 0.045;
			boneIterator->bind_pose().T()	*= 0.045;
			boneIterator->bone_space().T()	*= 0.045;
			boneIterator->absolute().T()	*= 0.045;
			boneIterator->bone_space().T()	*= 0.045;
		}

		//mRagdoll->set_bind_pose();

		OpenTissue::skinning::scale_skin(m_skin, 0.045);

		// Read in new materials.
		OpenTissue::skinning::material_xml_read("ModelData/XML/character1_skin_material.xml",     m_skin.m_material[0]);
		OpenTissue::skinning::material_xml_read("ModelData/XML/character1_ponytail_material.xml", m_skin.m_material[1]);
		OpenTissue::skinning::material_xml_read("ModelData/XML/character1_chest_material.xml",    m_skin.m_material[2]);
		OpenTissue::skinning::material_xml_read("ModelData/XML/character1_pelvis_material.xml",   m_skin.m_material[3]);

		m_time          = 0;
		m_delta_time    = 0.02;
		m_duration      = 0;

		m_display_bones = false;
		//m_display_skin  = true;


		//skin_type::init_skin_render();
		m_skin.init();

		//m_skin_render.init( m_skin );
		mRagdoll->setSkin(m_skin);

		// Create vertex array for GPU uploading.
		//m_skin_render.createGPUBuffers( m_skin );
	}

	void drawGUI(DemoHandler* draw)
	{
		Point drawPoint = Point(6.5f, 7.f, 0);

		//draw->drawText(drawPoint, "Cuboid velocity: " + PointToString(mCuboid->GetVelocity()));
		//draw->drawText(drawPoint, "Cuboid acceleration: " + PointToString(mCuboid->GetAcceleration()));
		drawNextText(drawPoint, "Delta time: "			+ to_string(mDT), draw);
		//draw->drawText(drawPoint, "Cuboid angular velocity (L): " + PointToString(mCuboid->GetAngularVelocity()));
		//draw->drawText(drawPoint, "Cuboid mass: " + to_string(mCuboid->GetMass()));
		drawNextText(drawPoint, "Gravity active: " + getString(mParticleManager->mIsGravityActive), draw);
		drawNextText(drawPoint, "Debug active: " + getString(mParticleManager->mDrawState), draw);
		drawNextText(drawPoint, "Gravity constant: " + getString(mParticleManager->mGravityAcceleration), draw);
		drawNextText(drawPoint, "Selected body part: " + mRagdoll->mBodyParts[mSelectedBodyPartIndex]->mName, draw);
		drawNextText(drawPoint, "Recording: " + getString(mParticleManager->isItRecording()), draw);
	}

	void drawNextText(Point& point, string text, DemoHandler* draw)
	{
		draw->drawText(point, text);
		point.mY -= 0.25;
	}

	const string getName() { return "Ragdoll"; }

	const string getInfo()
	{
		// Info shows only the 5 last rows of text.
		string r = "";
		r += "Press 1-3 to change the ragdoll rig.\n";
		r += "Press 4-8 to change the constraint solver. Relaxation, relaxation with priority, priority, others.\n";
        r += "Press g to toggle gravity. Press a, s, space or hold left click to apply force. \n";
        r += "Press d to change debug mode. Press j or k to change the selected body part. \n";
        r += "Press r to reset the scenario";
		return r;
	}
};

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0