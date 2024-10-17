#pragma once
#include "Actors.h"
#include "TurnThreshold.h"

class Tile;
class PathFindingComponent;

class Figurines : public Actors
{
public:
	Figurines(GameEngine &gameEngineP, std::string entityNameP, std::string nodeNameP, int ownerP);
	~Figurines();

	/* Updates */
	void Update(float deltaTimeP) override;
	void UpdatePositions(float deltaTimeP);

	/* Positions */
	Vector3 GetPosition() { return mNode->_getDerivedPosition(); }
	void SetPosition(Vector3 positionP);
	void SetYawRotation(Degree rotationP);
	bool IsSleeping() { return !mIsMoving; }

	/* Events */
	void OnSelected(bool isSelected);
	void OnMouseOver(bool isEnemy = false);
	void OnMouseOut();

	virtual void OnEndTurnEvent() override;

	/* Actions */
	void MoveTo(Tile *targetTileP);
	void Attack(Figurines *targetP);
	void GetHit(int damageAmountP);

	/* Getters */
	const float GetMovementAction() { return mCurrentMovementAction; }
	const int GetActionPoint() { return mCurrentActionPoint; }
	const int GetHealthPoint() { return mCurrentHealthPoint; }
	const float GetAttackRange() { return mAttackRange; }
	const int GetOwner() { return mOwnerID; }
	const bool IsDead() { return mIsDead; }

private:
	/* References */
	GameEngine &mGameEngine;

	bool mIsSelected = false;
	bool mIsEnemy = false;
	float mUniformScale = 0.02f;
	Vector3 mOffset = Vector3(0);
	int mOwnerID = 0;

	/* Figurine Stats */
	float mMaxMovementAction = 108.f;
	int mMaxHealthPoint = 2;
	float mAttackRange = 50.f;
	int mMaxActionPoint = 2;

	/* Current Stats */
	int mCurrentHealthPoint;
	float mCurrentMovementAction;
	int mCurrentActionPoint;

	/* On selected Animation properties */
	float mAnimationTime = 0.f;
	float mScaleSpeed = 2.f;
	float mScaleFactor = 0.002f;
	float mFlattenSpeed = 1.f;
	float mFlattenFactor = 0.001f;
	float mMovementSpeed = 10.f;

	/* On Movement */
	PathFindingComponent* mPathfinding = nullptr;
	std::vector<TurnThreshold*> mPath;
	Vector3 mStraightTargetPosition;
	int mIndexPosition = 1;
	bool mIsMoving = false;
	bool mMoveStraight = false;
	int mTurnSpeed = 4;

	/* On Death */
	bool mIsDead = false;
	void LookAt(const Ogre::Vector3& targetPositionP, float deltaTimeP);
};

