#include "Ragdoll.h"
#include "Include\OpenTissue\Kinematics\Skeleton\SkeletonTypes.h"
#include "ConstraintRodA.h"

using namespace pc;

template<typename skeleton_type_>
Ragdoll<skeleton_type_>::Ragdoll() : skeleton_type_(){}

template<typename skeleton_type_>
pc::Ragdoll<skeleton_type_>::~Ragdoll()
{
	for (BodyPart* bodyPart : mBodyParts)
		delete bodyPart;
	for (JointsBoneConnection* jointBoneConnection : mJointsBoneConnections)
		delete jointBoneConnection;

	if (mJointsBoneConnections.size() > 0)
		mJointsBoneConnections.clear();
	if (mJoints.size() > 0)
		mJoints.clear();
	if (mHinges.size() > 0)
		mHinges.clear();
	if (mNodes.size() > 0)
		mNodes.clear();
	if (mBodyParts.size() > 0)
		mBodyParts.clear();
	if (mParticles.size() > 0)
		mParticles.clear();
	if (mCuboids.size() > 0)
		mCuboids.clear();
}

template<typename skeleton_type_>
Ragdoll<skeleton_type_>* Ragdoll<skeleton_type_>::createRagdollV1(Vector startPosition, float mass)
{
	Ragdoll* ret = new Ragdoll();

	ret->mMass = mass;
	float dividedMass = mass / (8.0f * 12.0f);

	/// Cuboid body parts.
	ret->addCuboid(ret->mRightLowerLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 1.00, -0.3) + startPosition, 0.5, 2.2, 0.5, dividedMass, BLUE,	BLUE	), "right lower leg");
	ret->addCuboid(ret->mRightUpperLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 3.10, -0.3) + startPosition, 0.5, 2.0, 0.5, dividedMass, BLUE,	BLUE	), "right upper leg");
	ret->addCuboid(ret->mLeftLowerLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 1.00,  0.3) + startPosition, 0.5, 2.2, 0.5, dividedMass, BLUE,	BLUE	), "left lower leg");
	ret->addCuboid(ret->mLeftUpperLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 3.10,  0.3) + startPosition, 0.5, 2.0, 0.5, dividedMass, BLUE,	BLUE	), "left upper leg");
	ret->addCuboid(ret->mWaist			= ParticleCuboid::createParticleCuboidV3(Vector(-0.15, 4.55,  0.0) + startPosition, 0.8, 0.9, 1.1, dividedMass, BLUE,	BLUE	), "waist");
	ret->addCuboid(ret->mLowerTorso		= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 5.40,  0.0) + startPosition, 0.5, 0.8, 1.1, dividedMass, RED,	RED		), "lower torso");
	ret->addCuboid(ret->mUpperTorso		= ParticleCuboid::createParticleCuboidV3(Vector(-0.20, 6.20,  0.0) + startPosition, 0.9, 0.8, 1.1, dividedMass, RED,	RED		), "upper torso");
	ret->addCuboid(ret->mHead			= ParticleCuboid::createParticleCuboidV3(Vector(-0.50, 7.45,  0.0) + startPosition, 1.1, 1.3, 1.1, dividedMass, YELLOW, YELLOW	), "head");
	ret->addCuboid(ret->mRightUpperarm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 5.65, -0.8) + startPosition, 0.5, 1.9, 0.5, dividedMass, RED,	RED		), "right upper arm");
	ret->addCuboid(ret->mRightForearm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 3.75, -0.8) + startPosition, 0.5, 1.9, 0.5, dividedMass, RED,	RED		), "right forearm");
	ret->addCuboid(ret->mLeftUpperarm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 5.65,  0.8) + startPosition, 0.5, 1.9, 0.5, dividedMass, RED,	RED		), "left upper arm");
	ret->addCuboid(ret->mLeftForearm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.00, 3.75,  0.8) + startPosition, 0.5, 1.9, 0.5, dividedMass, RED,	RED		), "left forearm");

	ret->initJointsFromCuboids();

	return ret;
}

template<typename skeleton_type_>
Ragdoll<skeleton_type_>* Ragdoll<skeleton_type_>::createRagdollV2(Vector startPosition, float mass)
{
	Ragdoll<skeleton_type_>* ret = new Ragdoll<skeleton_type_>();

	ret->mMass = mass;
	float dividedMass = mass / (8.0f * 12.0f);

	/// Cuboid body parts.
	ret->addCuboid(ret->mRightLowerLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 0.993, -0.500) + startPosition, 0.5, 2.2, 0.5, dividedMass), "right lower leg");
	ret->addCuboid(ret->mRightUpperLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 3.159, -0.560) + startPosition, 0.5, 2.0, 0.5, dividedMass), "right upper leg");
	ret->addCuboid(ret->mLeftLowerLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 0.993,  0.500) + startPosition, 0.5, 2.2, 0.5, dividedMass), "left lower leg");
	ret->addCuboid(ret->mLeftUpperLeg	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 3.150,  0.560) + startPosition, 0.5, 2.0, 0.5, dividedMass), "left upper leg");
	ret->addCuboid(ret->mWaist			= ParticleCuboid::createParticleCuboidV3(Vector(-0.054, 4.683,  0.013) + startPosition, 0.8, 0.9, 1.3, dividedMass), "waist");
	ret->addCuboid(ret->mLowerTorso		= ParticleCuboid::createParticleCuboidV3(Vector( 0.043, 5.603,  0.013) + startPosition, 0.5, 0.8, 1.3, dividedMass), "lower torso");
	ret->addCuboid(ret->mUpperTorso		= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 6.485,  0.013) + startPosition, 0.9, 0.8, 1.5, dividedMass), "upper torso");
	ret->addCuboid(ret->mHead			= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 7.759,  0.013) + startPosition, 1.0, 1.3, 1.0, dividedMass), "head");
	ret->addCuboid(ret->mRightUpperarm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 5.946, -1.066) + startPosition, 0.5, 1.9, 0.5, dividedMass), "right upper arm");
	ret->addCuboid(ret->mRightForearm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 4.024, -1.066) + startPosition, 0.5, 1.9, 0.5, dividedMass), "right forearm");
	ret->addCuboid(ret->mLeftUpperarm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 5.946,  1.066) + startPosition, 0.5, 1.9, 0.5, dividedMass), "left upper arm");
	ret->addCuboid(ret->mLeftForearm	= ParticleCuboid::createParticleCuboidV3(Vector( 0.000, 4.024,  1.066) + startPosition, 0.5, 1.9, 0.5, dividedMass), "left forearm");

	ret->initJointsFromCuboids();

	return ret;
}

template<typename skeleton_type_>
Ragdoll<skeleton_type_>* Ragdoll<skeleton_type_>::createRagdollV3(Vector startPosition, float mass)
{
	Ragdoll* ret = new Ragdoll();

	ret->mMass = mass;
	float dividedMass = mass / (14.0f);

	/// Particles.
	ret->mParticles.push_back(new Particle(Vector( 0, -0.10, -0.3) + startPosition)); // p0. right toe (foot)
	ret->mParticles.push_back(new Particle(Vector( 0,  2.10, -0.3) + startPosition)); // p1. right knee
	ret->mParticles.push_back(new Particle(Vector( 0,  4.10, -0.3) + startPosition)); // p2. right hip
	ret->mParticles.push_back(new Particle(Vector( 0, -0.10,  0.3) + startPosition)); // p3. left toe (foot)
	ret->mParticles.push_back(new Particle(Vector( 0,  2.10,  0.3) + startPosition)); // p4. left knee
	ret->mParticles.push_back(new Particle(Vector( 0,  4.10,  0.3) + startPosition)); // p5. left hip
	ret->mParticles.push_back(new Particle(Vector( 0,  6.80,  0.0) + startPosition)); // p6. neck
	ret->mParticles.push_back(new Particle(Vector( 0,  7.45,  0.0) + startPosition)); // p7. head
	ret->mParticles.push_back(new Particle(Vector( 0,  4.70, -0.8) + startPosition)); // p8. right elbow
	ret->mParticles.push_back(new Particle(Vector( 0,  2.80, -0.8) + startPosition)); // p9. right hand
	ret->mParticles.push_back(new Particle(Vector( 0,  4.70,  0.8) + startPosition)); // p10. left elbow
	ret->mParticles.push_back(new Particle(Vector( 0,  2.80,  0.8) + startPosition)); // p11. left hand
	ret->mParticles.push_back(new Particle(Vector( 0,  6.60, -0.5) + startPosition)); // p12. right shoulder
	ret->mParticles.push_back(new Particle(Vector( 0,  6.60,  0.5) + startPosition)); // p13. left shoulder
	
	/// Joints.
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 0], ret->mParticles[ 1]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 1], ret->mParticles[ 2]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 3], ret->mParticles[ 4]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 4], ret->mParticles[ 5]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 2], ret->mParticles[ 5]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 2], ret->mParticles[ 6]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 2], ret->mParticles[12]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 2], ret->mParticles[13]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 5], ret->mParticles[ 6]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 5], ret->mParticles[12]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 5], ret->mParticles[13]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[12], ret->mParticles[13]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[12], ret->mParticles[ 6]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[13], ret->mParticles[ 6]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[12], ret->mParticles[ 8]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 8], ret->mParticles[ 9]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[13], ret->mParticles[10]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[10], ret->mParticles[11]));
	ret->mJoints.push_back(new ConstraintRodA(ret->mParticles[ 6], ret->mParticles[ 7]));

	/// Body parts.
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 0], ret->mParticles[ 1]}, "right lower leg"));										// bp0. right lower leg. p0, p1.
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 1], ret->mParticles[ 2]}, "right upper leg"));										// bp1. right upper leg. p1, p2. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 3], ret->mParticles[ 4]}, "left lower leg"));											// bp2. left upper leg. p3, p4. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 4], ret->mParticles[ 5]}, "left upper leg"));											// bp3. left lower leg. p4, p5. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 2], ret->mParticles[ 5]}, "waist"));													// bp4. waist. p2, p5. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 2], ret->mParticles[ 5], ret->mParticles[12], ret->mParticles[13]}, "lower torso"));	// bp5. lower torso. p2, p5, p12, p13.
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[12], ret->mParticles[13], ret->mParticles[6]}, "upper torso"));						// bp6. upper torso. p12, p13, p6.
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 7]}, "head"));																		// bp7. head. p7.
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 8], ret->mParticles[12]}, "right upper arm"));										// bp8. right upper arm. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[ 9], ret->mParticles[ 8]}, "right forearm"));											// bp9. right forearm. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[10], ret->mParticles[13]}, "left upper arm"));											// bp10. left upper arm. 
	ret->mBodyParts.push_back(new BodyPart(vector<Particle*>{ret->mParticles[11], ret->mParticles[10]}, "left forearm"));											// bp11. left forearm. 

	return ret;
}

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::initJointsFromCuboids()
{
	mCuboidMode = true;

	/// mJoints.
	mJoints.push_back(mRightHipJoint		= mCuboids[RIGHT_UPPER_LEG	]->createJoint(mCuboids[WAIST			]));
	mJoints.push_back(mLeftHipJoint			= mCuboids[LEFT_UPPER_LEG	]->createJoint(mCuboids[WAIST			]));
	mJoints.push_back(mRightArmJoint		= mCuboids[UPPER_TORSO		]->createJoint(mCuboids[RIGHT_UPPER_ARM	]));
	mJoints.push_back(mLeftArmJoint			= mCuboids[UPPER_TORSO		]->createJoint(mCuboids[LEFT_UPPER_ARM	]));

	/// mHinges.
	mHinges.push_back(mRightKneeHinge		= mCuboids[RIGHT_LOWER_LEG	]->createHinge(mCuboids[RIGHT_UPPER_LEG ]));
	mHinges.push_back(mLeftKneeHinge		= mCuboids[LEFT_LOWER_LEG	]->createHinge(mCuboids[LEFT_UPPER_LEG	]));
	mHinges.push_back(mRightElbowHinge		= mCuboids[RIGHT_FOREARM	]->createHinge(mCuboids[RIGHT_UPPER_ARM ]));
	mHinges.push_back(mLeftElbowHinge		= mCuboids[LEFT_FOREARM		]->createHinge(mCuboids[LEFT_UPPER_ARM	]));
	mHinges.push_back(mLowerTorsoHinge		= mCuboids[WAIST			]->createHinge(mCuboids[LOWER_TORSO		]));
	mHinges.push_back(mUpperTorsoHinge		= mCuboids[LOWER_TORSO		]->createHinge(mCuboids[UPPER_TORSO		]));
	mHinges.push_back(mNeckHinge			= mCuboids[UPPER_TORSO		]->createHinge(mCuboids[HEAD			]));

	// Color hinge and joint particles in their right colors
	for (auto joint : mHinges)
	{
		Color color = PINK;
		if (joint.first->mParticles[0]->getColor() == CYAN || joint.first->mParticles[0]->getColor() == GREEN)
			color = GREEN;
		joint.first->mParticles[0]->setColor(color);
		joint.first->mParticles[1]->setColor(color);
		joint.second->mParticles[0]->setColor(color);
		joint.second->mParticles[1]->setColor(color);
	}
	for (auto joint : mJoints)
	{
		Color color = CYAN;
		if (joint->mParticles[0]->getColor() == PINK || joint->mParticles[0]->getColor() == GREEN)
			color = GREEN;
		joint->mParticles[0]->setColor(color);
		joint->mParticles[1]->setColor(color);
	}

	/// mNodes.
	// End nodes.
	mNodes.push_back(mHeadTopNode			= new ConstraintRodA(mCuboids[HEAD				]->getSideDiagonalParticles(ParticleCuboid::Sides::TOP		)));
	mNodes.push_back(mLeftHandStartNode		= new ConstraintRodA(mCuboids[LEFT_FOREARM		]->getSideDiagonalParticles(ParticleCuboid::Sides::BOTTOM	)));
	mNodes.push_back(mRightHandStartNode	= new ConstraintRodA(mCuboids[RIGHT_FOREARM		]->getSideDiagonalParticles(ParticleCuboid::Sides::BOTTOM	)));
	mNodes.push_back(mLeftFootStartNode		= new ConstraintRodA(mCuboids[LEFT_LOWER_LEG	]->getSideDiagonalParticles(ParticleCuboid::Sides::BOTTOM	)));
	mNodes.push_back(mRightFootStartNode	= new ConstraintRodA(mCuboids[RIGHT_LOWER_LEG	]->getSideDiagonalParticles(ParticleCuboid::Sides::BOTTOM	)));
	
	// Spine nodes.
	mNodes.push_back(mSpineNode				= new ConstraintRodA(mCuboids[UPPER_TORSO]->getSideDiagonalParticles(ParticleCuboid::Sides::TOP		)));
	mNodes.push_back(mSpine1Node			= new ConstraintRodA(mCuboids[UPPER_TORSO]->getSideDiagonalParticles(ParticleCuboid::Sides::BOTTOM	)));
	mNodes.push_back(mSpine2Node			= new ConstraintRodA(mCuboids[LOWER_TORSO]->getSideDiagonalParticles(ParticleCuboid::Sides::BOTTOM	)));
}

template<typename skeleton_type_>
void Ragdoll<skeleton_type_>::addCuboid(ParticleCuboid* bodypart, string name)
{
	mBodyParts.push_back(new BodyPart(bodypart, name));
	mCuboids.push_back(bodypart);
}

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::updateBonePositions()
{
	skeleton_type_* skeleton = (skeleton_type_*)this;
	int j = 0;

	for (auto i = mJointsBoneConnections.begin(); i != mJointsBoneConnections.end(); ++i)
	{
		//if (j < 9 || j > 17) { ++j; continue; }
		vector3_type vectorFirst = vector3_type(
			(*i)->mJointFirst->getCenterPosition().mX,
			(*i)->mJointFirst->getCenterPosition().mY,
			(*i)->mJointFirst->getCenterPosition().mZ);
		vector3_type vectorSecond = vector3_type(
			(*i)->mJointSecond->getCenterPosition().mX,
			(*i)->mJointSecond->getCenterPosition().mY,
			(*i)->mJointSecond->getCenterPosition().mZ);
		(*i)->mBone->bone_space_transform().Q().lookAt(vectorFirst, vectorSecond, vector3_type(0, 0, 1), vector3_type(0, 1, 0));
		// Convert axis angle to quaternion.
		quaternion_type result;
		result.Ry(M_PI / 2);
		//(*i)->mBone->bone_space_transform().Q() = (*i)->mBone->bone_space_transform().Q() % result;
		//skeleton->compute_pose();
		++j;
	}

	skeleton->compute_pose();

	j = 0;

	for (auto i = mJointsBoneConnections.begin(); i != mJointsBoneConnections.end(); ++i)
	{
		//if (j < 9 || j > 17) { ++j; continue; }		

		Vector middlePosition = ((*i)->mJointFirst->getCenterPosition() +
			(*i)->mJointSecond->getCenterPosition()) * 0.5f;
		(*i)->mBone->bone_space_transform().T() =
			vector3_type(middlePosition.mX, middlePosition.mY, middlePosition.mZ) -
			(*i)->mBone->bone_space_transform().Q().rotate((*i)->mBone->absolute().T());

		++j;
	}
}

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::draw(DemoHandler* draw, const bool drawDebug)
{
	if (mRenderSkin)
	{
		updateBonePositions();
		m_skin_render.render(m_skin, *this);
	}
}

template<typename skeleton_type_>
void Ragdoll<skeleton_type_>::addForce(int index, Vector f)
{
	// TODO.
	Vector dividedForce = f * (1.0f / (float)mBodyParts[index]->mParticles.size());
	for (Particle* particle : mBodyParts[index]->mParticles)
		particle->addForce(dividedForce);
}

template<typename skeleton_type_>
void Ragdoll<skeleton_type_>::initBones()
{
	mJointsBoneConnections.clear();

	if (mCuboidMode)
	{
		skeleton_type_* skeleton = (skeleton_type_*)this;

		typename skeleton_type_::bone_iterator boneIterator = skeleton->begin();	

		// Middle parts of the body.
		mBoneMain			= skeleton->get_bone("Cally");
		//mJointsBoneConnections.push_back(new JointsBoneConnection(&mLowerTorsoHinge,	&mLowerTorsoHinge,	mBoneMain));
		mBonePelvis			= skeleton->get_bone("Cally Pelvis");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHipJoint,		mRightHipJoint,		mBonePelvis));
		mBoneSpine			= skeleton->get_bone("Cally Spine");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mSpineNode,			mSpineNode,			mBoneSpine));
		mBoneSpine1			= skeleton->get_bone("Cally Spine1");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mSpine1Node,			mSpine1Node,		mBoneSpine1));
		mBoneSpine2			= skeleton->get_bone("Cally Spine2");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mSpine2Node,			mSpine2Node,		mBoneSpine2));
		mBoneNeck			= skeleton->get_bone("Cally Neck");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mUpperTorsoHinge,	mHeadTopNode,		mBoneNeck));
		mBoneHead			= skeleton->get_bone("Cally Head");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mUpperTorsoHinge,	mHeadTopNode,		mBoneHead));
		mBonePonytail1		= skeleton->get_bone("Cally Ponytail1");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mUpperTorsoHinge,	mHeadTopNode,		mBonePonytail1));
		mBonePonytail11		= skeleton->get_bone("Cally Ponytail11");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mUpperTorsoHinge,	mHeadTopNode,		mBonePonytail11));

		// Upper left part of the body.
		mBoneLeftClavicle	= skeleton->get_bone("Cally L Clavicle");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mUpperTorsoHinge,	mLeftArmJoint,		mBoneLeftClavicle));
		mBoneLeftUpperArm	= skeleton->get_bone("Cally L UpperArm");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftArmJoint,		&mLeftElbowHinge,	mBoneLeftUpperArm));
		mBoneLeftForearm	= skeleton->get_bone("Cally L Forearm");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mLeftElbowHinge,		mLeftHandStartNode, mBoneLeftForearm));
		mBoneLeftHand		= skeleton->get_bone("Cally L Hand");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftHand));
		mBoneLeftFinger0	= skeleton->get_bone("Cally L Finger0");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftFinger0));
		mBoneLeftFinger01	= skeleton->get_bone("Cally L Finger01");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftFinger01));
		mBoneLeftFinger02	= skeleton->get_bone("Cally L Finger02");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftFinger02));
		mBoneLeftFinger1	= skeleton->get_bone("Cally L Finger1");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftFinger1));
		mBoneLeftFinger11	= skeleton->get_bone("Cally L Finger11");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftFinger11));
		mBoneLeftFinger12	= skeleton->get_bone("Cally L Finger12");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHandStartNode,	mLeftHandStartNode, mBoneLeftFinger12));

		// Upper right part of the body.
		mBoneRightClavicle	= skeleton->get_bone("Cally R Clavicle");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mUpperTorsoHinge,	mRightArmJoint,		 mBoneRightClavicle));
		mBoneRightUpperArm	= skeleton->get_bone("Cally R UpperArm");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightArmJoint,		&mRightElbowHinge,	 mBoneRightUpperArm));
		mBoneRightForearm	= skeleton->get_bone("Cally R Forearm");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mRightElbowHinge,	mRightHandStartNode, mBoneRightForearm));
		mBoneRightHand		= skeleton->get_bone("Cally R Hand");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightHand));
		mBoneRightFinger0	= skeleton->get_bone("Cally R Finger0");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightFinger0));
		mBoneRightFinger01	= skeleton->get_bone("Cally R Finger01");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightFinger01));
		mBoneRightFinger02	= skeleton->get_bone("Cally R Finger02");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightFinger02));
		mBoneRightFinger1	= skeleton->get_bone("Cally R Finger1");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightFinger1));
		mBoneRightFinger11	= skeleton->get_bone("Cally R Finger11");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightFinger11));
		mBoneRightFinger12	= skeleton->get_bone("Cally R Finger12");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightHandStartNode,	mRightHandStartNode, mBoneRightFinger12));

		// Lower part of the body.
		mBoneLeftThigh		= skeleton->get_bone("Cally L Thigh");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHipJoint,		&mLeftKneeHinge,	 mBoneLeftThigh));
		mBoneLeftCalf		= skeleton->get_bone("Cally L Calf");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mLeftKneeHinge,		mLeftFootStartNode,  mBoneLeftCalf));
		mBoneLeftFoot		= skeleton->get_bone("Cally L Foot");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftFootStartNode,	mLeftFootStartNode,  mBoneLeftFoot));
		mBoneLeftToe0		= skeleton->get_bone("Cally L Toe0");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftFootStartNode,	mLeftFootStartNode,  mBoneLeftToe0));

		mBoneRightThigh		= skeleton->get_bone("Cally R Thigh");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mLeftHipJoint,		&mLeftKneeHinge,	 mBoneRightThigh));
		mBoneRightCalf		= skeleton->get_bone("Cally R Calf");
		mJointsBoneConnections.push_back(new JointsBoneConnection(&mRightKneeHinge,		mRightFootStartNode, mBoneRightCalf));
		mBoneRightFoot		= skeleton->get_bone("Cally R Foot");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightFootStartNode,	mRightFootStartNode, mBoneRightFoot));
		mBoneRightToe0		= skeleton->get_bone("Cally R Toe0");
		mJointsBoneConnections.push_back(new JointsBoneConnection(mRightFootStartNode,	mRightFootStartNode, mBoneRightToe0));

		for (++boneIterator; boneIterator != skeleton->end(); ++boneIterator)
		{
			//boneIterator->bone_space_transform().Q().rotate(0, M_PI / 2, -M_PI / 2);//.Rx(-M_PI / 2);
			boneIterator->bone_space_transform().Q().rotate(0, M_PI / 2, -M_PI / 2);
			boneIterator->bone_space_transform().T() += vector3_type(4.5, 5, 0);
		
			//updateBonePositions();
		}

		//skeleton->compute_pose();
	}
}

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::drawBoneJointConnections(DemoHandler *draw)
{
	if (mCuboidMode)
	{
		draw->drawPoint(mHeadTopNode->getCenterPosition().getPoint(), MAGENTA);
		draw->drawPoint(mLeftHandStartNode->getCenterPosition().getPoint(), MAGENTA);
		draw->drawPoint(mRightHandStartNode->getCenterPosition().getPoint(), MAGENTA);
		draw->drawPoint(mLeftFootStartNode->getCenterPosition().getPoint(), MAGENTA);
		draw->drawPoint(mRightFootStartNode->getCenterPosition().getPoint(), MAGENTA);

		int i = 0;
		for (auto jointBoneConnection : mJointsBoneConnections)
		{
			Vector position = (jointBoneConnection->mJointFirst->getCenterPosition() + jointBoneConnection->mJointSecond->getCenterPosition()) * 0.5f;
			switch (i)
			{
			case 1:
				draw->drawPoint(position.getPoint(), YELLOW);
				break;
			case 2:
				draw->drawPoint(position.getPoint(), CYAN);
				break;
			case 3:
				draw->drawPoint(position.getPoint(), PINK);
				break;
			default:
				draw->drawPoint(position.getPoint(), GREEN);
				break;
			}
			++i;
		}
	}
}

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::printBoneInfo()
{
	bone_iterator boneIterator = skeleton_type_::get_bone_iterator(0);

	int iterations = 0;
	int maxIterations = 100;
	while (boneIterator._Ptr != nullptr && iterations++ < maxIterations)
	{
		bone_type bone = (bone_type)boneIterator._Ptr->_Myval;
		string parentName = "<None>", boneNumberString = "", parentNumberString = "<None>";

		if (bone.parent() != nullptr)
		{
			parentName			= bone.parent()->get_name();
			parentNumberString	= to_string(bone.parent()->get_number());
		}

		boneNumberString = to_string(bone.get_number());

		// Correct line spacings.
		if (boneNumberString.length() < 2)
			boneNumberString.push_back(' ');
		if (parentNumberString.length() < 2)
			parentNumberString.push_back(' ');

		cout << "Bone number: " << boneNumberString << "" << ", bone name: " << bone.get_name()
			 << "\tparent number: " << parentNumberString << ", parent name: " << parentName << endl;

		if (boneIterator._Ptr->_Myval.get_number() == getBoneSize() - 1)
			break;
		boneIterator._Ptr = boneIterator._Ptr->_Next;
	}
}

template<typename skeleton_type_>
Vector Ragdoll<skeleton_type_>::getPosition(int index) const
{
	// TODO.
	Vector res = Vector();
	for (Particle* particle : mBodyParts[index]->mParticles)
		res += particle->getPosition();
	return res * (1.0f / (float)mBodyParts[index]->mParticles.size());
}

template<typename skeleton_type_>
float Ragdoll<skeleton_type_>::getMass() const { return mMass; }

template<typename skeleton_type_>
int pc::Ragdoll<skeleton_type_>::getBoneSize() const { return skeleton_type_::size(); }

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::setSkin(skin_type &skin)
{
	m_skin = skin;
	m_skin_render.init(skin);
}

template<typename skeleton_type_>
void pc::Ragdoll<skeleton_type_>::toggleRenderSkin(){ mRenderSkin = !mRenderSkin; }

template class Ragdoll<OpenTissue::skeleton::Types< OpenTissue::math::default_math_types >::skeleton_type>;

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0
// Inspired by Cyclone physics system https://github.com/idmillington/cyclone-physics