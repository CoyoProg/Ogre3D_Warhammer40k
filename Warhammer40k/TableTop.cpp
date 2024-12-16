#include "TableTop.h"
#include "QueryFlags.h"

TableTop::TableTop(GameManager& gameManagerP) :
	mGameManager(gameManagerP)
{
	SceneManager& sceneManager = gameManagerP.GetSceneManager();

	/* Import Custom mesh */
	Entity* tabletopEntity = sceneManager.createEntity("TableTopEntity", "Plateau.mesh");
	tabletopEntity->setMaterialName("Plateau.bois");

	/* Table Top */
	mNode = sceneManager.getRootSceneNode()->createChildSceneNode("CenterOfGameNode");
	mNode->setPosition(Vector3(0, 0, 0));

	SceneNode* secondNode = mNode->createChildSceneNode("TableTopNode");
	secondNode->attachObject(tabletopEntity);
	secondNode->setScale(10, 2.5, 10);
	secondNode->yaw(Degree(90));
}

void TableTop::Update(float deltaTimeP)
{
	Actors::Update(deltaTimeP);

	if (mFlipAnimation.isFlipping)
	{
		FlipTableTop(deltaTimeP);
	}
}

void TableTop::OnEndTurn()
{
	mFlipAnimation.isFlipping = true;
}

void TableTop::FlipTableTop(float deltaTimeP)
{
	if (mFlipAnimation.isFlipping)
	{
		UpdateAnimationProgress(deltaTimeP);
		UpdateBumpEffect();
		UpdateRotationEffect();

		/* Check if the flip is complete */
		if (mFlipAnimation.rotationAngle > 179)
		{
			ResetAnimationProperties();
			ResetTableTransform();

			mGameManager.OnNewTurn();
		}
	}
}

void TableTop::ResetAnimationProperties()
{
	mFlipAnimation.flipFlop = -mFlipAnimation.flipFlop;
	mFlipAnimation.animationProgress = 0.0f;
	mFlipAnimation.isFlipping = false;
	mFlipAnimation.shouldChangeDir = false;
	mFlipAnimation.animationSpeed = 0.f;
}

void TableTop::ResetTableTransform()
{
	/* Snap TableTop orientation */
	Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
	if (mFlipAnimation.flipFlop == -1)
	{
		orientation.FromAngleAxis(Degree(180), Ogre::Vector3::UNIT_Y);
	}
	else
	{
		orientation.FromAngleAxis(Degree(0), Ogre::Vector3::UNIT_Y);
	}
	mNode->setOrientation(orientation);

	/* Snap TableTop position */
	Vector3 finalPos = mNode->getPosition();
	mNode->setPosition(Vector3(finalPos.x, 0, finalPos.z));
}

void TableTop::UpdateRotationEffect()
{
	/* Rotation Angle */
	mFlipAnimation.rotationAngle = sin(mFlipAnimation.animationProgress * mFlipAnimation.animationSpeed / 2) * 180;
	Quaternion orientation = Ogre::Quaternion::IDENTITY;
	if (mFlipAnimation.flipFlop == -1)
		orientation.FromAngleAxis(Degree(mFlipAnimation.rotationAngle + 180), Ogre::Vector3::UNIT_Y);
	else
		orientation.FromAngleAxis(Degree(mFlipAnimation.rotationAngle), Ogre::Vector3::UNIT_Y);

	mNode->setOrientation(orientation);
}

void TableTop::UpdateBumpEffect()
{
	/* Bump Height */
	float translationY = sin(mFlipAnimation.animationProgress * mFlipAnimation.animationSpeed) * mFlipAnimation.bumpHeight;
	Vector3 newPos = mNode->getPosition();
	newPos.y = translationY;

	mNode->setPosition(newPos);
}

void TableTop::UpdateAnimationProgress(float deltaTimeP)
{
	if (mFlipAnimation.animationProgress > -0.28f && !mFlipAnimation.shouldChangeDir)
	{
		mFlipAnimation.animationProgress -= deltaTimeP;

		if (mFlipAnimation.animationSpeed < 1.f)
		{
			mFlipAnimation.animationSpeed += deltaTimeP;
		}
		else
		{
			mFlipAnimation.animationSpeed = 1.f;
		}
	}
	else
	{
		mFlipAnimation.shouldChangeDir = true;
		mFlipAnimation.animationProgress += deltaTimeP;

		if (mFlipAnimation.animationSpeed < 5.f)
		{
			mFlipAnimation.animationSpeed += deltaTimeP * 6;
		}
		else
		{
			mFlipAnimation.animationSpeed = 5.f;
		}
	}
}
