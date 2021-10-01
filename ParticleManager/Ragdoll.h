#pragma once
#include "Entity.h"
#include "Vector.h"
#include "ParticleCuboid.h"
#include <OpenTissue\Kinematics\Skinning\SkinningTypes.h>
#include <OpenTissue\Kinematics\Skeleton\SkeletonTypes.h>
#include <OpenTissue\Kinematics\Skeleton\SkeletonSkeleton.h>
#include <OpenTissue\Core\Math\MathBasicTypes.h>


namespace pc
{
#pragma region Enums, Classes and Structs
	enum BodyPartIndex
	{
		RIGHT_LOWER_LEG,	//  0. right lower leg
		RIGHT_UPPER_LEG,	//  1. right upper leg
		LEFT_LOWER_LEG,		//  2. left lower leg
		LEFT_UPPER_LEG,		//  3. left upper Leg
		WAIST,				//  4. waist
		LOWER_TORSO,		//  5. lower torso
		UPPER_TORSO,		//  6. upper torso
		HEAD,				//  7. head
		RIGHT_UPPER_ARM,	//  8. right upper arm
		RIGHT_FOREARM,		//  9. right forearm
		LEFT_UPPER_ARM,		// 10. left upper arm
		LEFT_FOREARM,		// 11. left forearm

		/* Used with stickman */
		
		RIGHT_HAND,			// 12. right hand
		LEFT_HAND,			// 13. left hand
		RIGHT_FOOT,			// 14. right foot
		LEFT_FOOT			// 15. left foot
	};
	
	struct BodyPart
	{
		vector<Particle*> mParticles;
		string mName = "";
		//pair<Particle*, Particle*>* mConnectionPoint;

		BodyPart(ParticleCuboid* cuboid, string name)
		{
			for (Particle* p : cuboid->mParticles)
				mParticles.push_back(p);
			mName = name;
		}

		BodyPart(vector<Particle*> particles, string name)
		{
			for (Particle* p : particles)
				mParticles.push_back(p);
			mName = name;
		}
	};

	struct Joint
	{
		ConstraintRod* mConstraintFirst;
		ConstraintRod* mConstraintSecond;

		Joint(ConstraintRod* constraintFirst, ConstraintRod* constraintSecond)
		{
			mConstraintFirst	= constraintFirst;
			mConstraintSecond	= constraintSecond;
		}

		virtual Vector getCenterPosition() = 0;
	};

	struct BallJoint : public Joint
	{
		BallJoint(ConstraintRod* constraint) : Joint(constraint, nullptr){}

		virtual Vector getCenterPosition(){ return mConstraintFirst->getCenterPosition(); }
	};

	struct HingeJoint : public Joint
	{
		HingeJoint(ConstraintRod* constraintFirst, ConstraintRod* constraintSecond) : Joint(constraintFirst, constraintSecond){}

		virtual Vector getCenterPosition()
		{
			Vector constraintFirstPosition	= mConstraintFirst->getCenterPosition();
			Vector constraintSecondPosition = mConstraintSecond->getCenterPosition();

			float midX = (constraintFirstPosition.mX + constraintSecondPosition.mX) / 2;
			float midY = (constraintFirstPosition.mY + constraintSecondPosition.mY) / 2;
			float midZ = (constraintFirstPosition.mZ + constraintSecondPosition.mZ) / 2;

			return Vector(midX, midY, midZ);
		}
	};

	struct JointsBoneConnection
	{
		typedef OpenTissue::math::default_math_types		math_types;
		typedef OpenTissue::skeleton::Types< math_types >	skeleton_types;
		typedef skeleton_types::bone_type					bone_type;

		Joint*		mJointFirst;
		Joint*		mJointSecond;
		bone_type*	mBone;

		JointsBoneConnection(Joint* jointFirst, Joint* jointSecond, bone_type* bone)
		{
			mJointFirst		= jointFirst;
			mJointSecond	= jointSecond;
			mBone			= bone;
		}

		// Only ball joints.
		JointsBoneConnection(ConstraintRod* ballJointFirst, ConstraintRod* ballJointSecond, bone_type* bone)
		{
			mJointFirst		= new BallJoint(ballJointFirst);
			mJointSecond	= new BallJoint(ballJointSecond);
			mBone			= bone;
		}

		// Only hinge joints.
		JointsBoneConnection(pair<ConstraintRod*, ConstraintRod*>* hingeJointFirst,
			pair<ConstraintRod*, ConstraintRod*>* hingeJointSecond, bone_type* bone)
		{
			mJointFirst		= new HingeJoint(hingeJointFirst->first, hingeJointFirst->second);
			mJointSecond	= new HingeJoint(hingeJointSecond->first, hingeJointSecond->second);
			mBone			= bone;
		}

		// Both a ball and a hinge joint (ball joint first).
		JointsBoneConnection(ConstraintRod* ballJoint, pair<ConstraintRod*, ConstraintRod*>* hingeJoint, bone_type* bone)
		{
			mJointFirst		= new BallJoint(ballJoint);
			mJointSecond	= new HingeJoint(hingeJoint->first, hingeJoint->second);
			mBone			= bone;
		}

		// Both a ball and a hinge joint (hinge joint first).
		JointsBoneConnection(pair<ConstraintRod*, ConstraintRod*>* hingeJoint, ConstraintRod* ballJoint, bone_type* bone)
		{
			mJointFirst		= new HingeJoint(hingeJoint->first, hingeJoint->second);
			mJointSecond	= new BallJoint(ballJoint); 
			mBone			= bone;
		}
	};	
#pragma endregion Enums, Classes and Structs

	template<typename skeleton_type_>
	class Ragdoll : public Entity, public skeleton_type_
	{
		Color	mColor;
		float	mMass;
		bool	mCuboidMode = false;
		//vector<vector<unsigned>> mFaces;
	public:
		typedef OpenTissue::math::default_math_types	math_types;
		typedef math_types::real_type					real_type;
		typedef typename skeleton_type_::bone_type		bone_type;
		typedef typename skeleton_type_::bone_iterator	bone_iterator;
		typedef math_types::vector3_type				vector3_type;
		typedef math_types::matrix3x3_type				matrix3x3_type;
		typedef math_types::quaternion_type				quaternion_type;

		typedef OpenTissue::skinning::Types< math_types, OpenTissue::skinning::DBS >	skin_types;
		typedef skin_types::skin_type													skin_type;
		typedef skin_types::skin_render_type											skin_render_type;

	private:
		skin_render_type	m_skin_render;
		skin_type			m_skin;			///< Skin container.
		bool				mRenderSkin = false;
	#pragma region Bones
		bone_type* mBoneMain;
		bone_type* mBonePelvis;
		bone_type* mBoneSpine;
		bone_type* mBoneSpine1;
		bone_type* mBoneSpine2;
		bone_type* mBoneNeck;
		bone_type* mBoneHead;
		bone_type* mBonePonytail1;
		bone_type* mBonePonytail11;
		bone_type* mBoneLeftClavicle;
		bone_type* mBoneLeftUpperArm;
		bone_type* mBoneLeftForearm;
		bone_type* mBoneLeftHand;
		bone_type* mBoneLeftFinger0;
		bone_type* mBoneLeftFinger01;
		bone_type* mBoneLeftFinger02;
		bone_type* mBoneLeftFinger1;
		bone_type* mBoneLeftFinger11;
		bone_type* mBoneLeftFinger12;
		bone_type* mBoneRightClavicle;
		bone_type* mBoneRightUpperArm;
		bone_type* mBoneRightForearm;
		bone_type* mBoneRightHand;
		bone_type* mBoneRightFinger0;
		bone_type* mBoneRightFinger01;
		bone_type* mBoneRightFinger02;
		bone_type* mBoneRightFinger1;
		bone_type* mBoneRightFinger11;
		bone_type* mBoneRightFinger12;
		bone_type* mBoneLeftThigh;
		bone_type* mBoneLeftCalf;
		bone_type* mBoneLeftFoot;
		bone_type* mBoneLeftToe0;
		bone_type* mBoneRightThigh;
		bone_type* mBoneRightCalf;
		bone_type* mBoneRightFoot;
		bone_type* mBoneRightToe0;
		vector<JointsBoneConnection*> mJointsBoneConnections;		
	#pragma endregion Bones

		void addCuboid(ParticleCuboid* bodypart, string name);
		void initJointsFromCuboids();
	public:
		Ragdoll();
		~Ragdoll();
		vector<Particle*> mParticles;
		vector<ParticleCuboid*> mCuboids;
		vector<ConstraintRod*> mJoints;
		vector<pair<ConstraintRod*, ConstraintRod*>> mHinges;
		// Their to help connect the end points of the body with the end points of the bones.
		vector<ConstraintRod*> mNodes;
		vector<BodyPart*> mBodyParts;
		/// Body parts.
		ParticleCuboid* mHead;
		ParticleCuboid* mRightLowerLeg;
		ParticleCuboid* mRightUpperLeg;
		ParticleCuboid* mLeftLowerLeg;
		ParticleCuboid* mLeftUpperLeg;
		ParticleCuboid* mWaist;
		ParticleCuboid* mLowerTorso;
		ParticleCuboid* mUpperTorso;
		ParticleCuboid* mRightUpperarm;
		ParticleCuboid* mRightForearm;
		ParticleCuboid* mLeftUpperarm;
		ParticleCuboid* mLeftForearm;
		/// Joints.
		// Left side.
		ConstraintRod* mLeftHipJoint;
		ConstraintRod* mLeftArmJoint;
		// Right side.
		ConstraintRod* mRightHipJoint;
		ConstraintRod* mRightArmJoint;
		/// Hinges.
		// Left side.
		pair<ConstraintRod*, ConstraintRod*> mLeftKneeHinge;
		pair<ConstraintRod*, ConstraintRod*> mLeftElbowHinge;
		// Right side.
		pair<ConstraintRod*, ConstraintRod*> mRightKneeHinge;
		pair<ConstraintRod*, ConstraintRod*> mRightElbowHinge;
		// Body.
		pair<ConstraintRod*, ConstraintRod*> mLowerTorsoHinge;
		pair<ConstraintRod*, ConstraintRod*> mUpperTorsoHinge;
		pair<ConstraintRod*, ConstraintRod*> mNeckHinge;
		// End nodes.
		ConstraintRod* mHeadTopNode;
		ConstraintRod* mLeftHandStartNode;
		ConstraintRod* mRightHandStartNode;
		ConstraintRod* mLeftFootStartNode;
		ConstraintRod* mRightFootStartNode;
		ConstraintRod* mSpineNode;
		ConstraintRod* mSpine1Node;
		ConstraintRod* mSpine2Node;

		// Creates a ragdoll made out of cuboids where each adjacent edge at the hinge joints are of same length.
		static Ragdoll* createRagdollV1(Vector startPosition, float mass = 12.0f);
		// Creates a ragdoll made out of cuboids with same measurement as Ian Millingtons Cyclone Physics Project.
		static Ragdoll* createRagdollV2(Vector startPosition, float mass = 12.0f);
		// Creates a stickman ragdoll made out of particles and rod constraints.
		static Ragdoll* createRagdollV3(Vector startPosition, float mass = 14.0f);

		virtual void draw(DemoHandler* draw, const bool drawDebug = false);
		void	addForce(int index, Vector f);
		
		void	initBones();
		void	drawBoneJointConnections(DemoHandler* draw);
		void	printBoneInfo();
		void	updateBonePositions();

		Vector	getPosition(int index) const;
		float	getMass() const;
		int		getBoneSize() const;

		void	setSkin(skin_type &skin);
		void	toggleRenderSkin();
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0
// Inspired by Cyclone physics system https://github.com/idmillington/cyclone-physics