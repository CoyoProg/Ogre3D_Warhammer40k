#pragma once
#include "Actors.h"
#include "TurnThreshold.h"

class Tile;
class PathFindingComponent;

/*
 * The figurines can be moved by the current player,
 * They all have the same stats and can move and shoot at an enemy
 */
class Figurines : public Actors
{
public:
	Figurines(GameEngine &gameEngineP, std::string entityNameP, std::string nodeNameP, int ownerP);
	~Figurines();

	void Update(float deltaTimeP) override;

	/* Update the Scale of the Figurine to create the SelectedAnimation */
	void UpdateSelectedAinamtion(float deltaTimeP);
	void UpdatePositions(float deltaTimeP);

	const Vector3& GetPosition() const { return mNode->_getDerivedPosition(); }
	void SetPosition(Vector3 positionP);
	void SetYawRotation(const Degree& rotationP);
	bool IsSleeping() const { return !mIsMoving; }

	float GetMovementAction() const { return mCurrentMovementAction; }
	int GetActionPoints() const { return mCurrentActionPoint; }
	int GetHealthPoints() const { return mCurrentHealthPoint; }
	float GetAttackRange() const { return ATTACK_RANGE; }
	int GetOwner() const { return mOwnerID; }
	bool IsDead() const { return mIsDead; }

	void MoveTo(Tile* targetTileP);
	void Attack(Figurines* targetP);
	void GetHit(int damageAmountP);

	void OnSelected(bool isSelected);
	void OnMouseOver(bool isEnemy = false);
	void OnMouseOut();

	virtual void OnEndTurnEvent() override;

private:
	void LookAt(const Ogre::Vector3& targetPositionP, float deltaTimeP);

	GameEngine &mGameEngine;

	bool mIsSelected = false;
	const float mUniformScale = 0.02f;
	Vector3 mPositionOffset = Vector3(0);

	/* The player ID that owns this figurine */
	int mOwnerID;

	const float MAX_MOVEMENT_ACTION = 100.f;
	const int MAX_HEALTH_POINTS = 2;
	const float ATTACK_RANGE = 50.f;
	const int MAX_ACTION_POINTS = 2;

	int mCurrentHealthPoint;
	float mCurrentMovementAction;
	int mCurrentActionPoint;

	float mSelectedAnim_Time = 0;
	float mSelectedAnim_ScaleSpeed = 2.f;
	float mSelectedAnim_ScaleFactor = 0.002f;
	float mSelectedAnim_FlattenSpeed = 1.f;
	float mSelectedAnim_FlattenFactor = 0.001f;
	float mSelectedAnim_MovementSpeed = 10.f;

	PathFindingComponent* mPathfinding;
	std::vector<TurnThreshold*> mPath;
	Vector3 mStraightTargetPosition;
	int mIndexPosition = 1;
	bool mIsMoving = false;
	bool mShouldMoveStraight = false;
	int mTurnSpeed = 4;

	/* On Death */
	bool mIsDead = false;
};

