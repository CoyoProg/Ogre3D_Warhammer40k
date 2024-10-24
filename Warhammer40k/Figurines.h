#pragma once
#include "Actors.h"
#include "TurnThreshold.h"

class Tile;
class PathFindingComponent;

struct FigurineStats 
{
	static constexpr float maxMovementAction = 100.f;
	static constexpr int maxHealthPoints = 2;
	static constexpr int maxActionPoints = 2;

	static constexpr int attackDamage = 1;
	static constexpr float attackRange = 50.f;
};

/*
 * @brief The figurines can be moved by the current player,
 * they all have the same stats and can move and shoot at an enemy
 */
class Figurines : public Actors
{
public:
	Figurines(GameEngine &gameEngineP, const std::string &entityNameP, const std::string &nodeNameP, int ownerP);
	~Figurines();

	void Update(float deltaTimeP) override;

	/* Update the Scale of the Figurine to create the SelectedAnimation */
	void UpdateSelectedAnimation(float deltaTimeP);
	void UpdatePositions(float deltaTimeP);

	Vector3 GetPosition() const { return mNode->_getDerivedPosition(); }
	void SetPosition(Vector3 positionP);
	void SetYawRotation(const Degree& rotationP);
	bool IsSleeping() const { return !mIsMoving; }

	float GetMovementAction() const { return mCurrentMovementAction; }
	int GetActionPoints() const { return mCurrentActionPoint; }
	int GetHealthPoints() const { return mCurrentHealthPoint; }
	float GetAttackRange() const { return FigurineStats::attackRange; }
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

	int mCurrentHealthPoint;
	float mCurrentMovementAction;
	int mCurrentActionPoint;

	float mSelectedAnim_Time = 0;
	static constexpr float ANIM_SCALE_SPEED = 2.f;
	static constexpr float ANIM_SCALE_FACTOR = 0.002f;
	static constexpr float ANIM_FLATTEN_SPEED = 1.f;
	static constexpr float ANIM_FLATTEN_FACTOR = 0.001f;
	static constexpr float ANIM_MOVEMENT_SPEED = 10.f;

	PathFindingComponent* mPathfinding = nullptr;
	std::vector<TurnThreshold*> mPath;
	Vector3 mStraightTargetPosition;
	int mIndexPosition = 1;
	bool mIsMoving = false;
	bool mShouldMoveStraight = false;
	int mTurnSpeed = 4;

	/* On Death */
	bool mIsDead = false;
};

