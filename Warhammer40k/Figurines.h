#pragma once
#include "Actors.h"
#include "TurnThreshold.h"

class PathFindingComponent;

class Figurines : public Actors
{
public:
	Figurines(GameEngine& gameEngineP, std::string entityNameP, std::string nodeNameP, int owner);
	~Figurines();

	/* Updates */
	void Update(float deltaTime) override;
	void UpdatePositions(float deltaTime);

	/* Positions */
	Vector3 GetPosition() { return m_Node->_getDerivedPosition(); }
	void SetPosition(Vector3 positionP);
	void SetYawRotation(Degree rotationP);
	bool IsSleeping() { return !m_IsMoving; }

	/* Events */
	void OnSelected(bool isSelected);
	void OnMouseOver(bool isEnemy = false);
	void OnMouseOut();

	virtual void OnEndTurnEvent() override;

	/* Actions */
	void MoveTo(Vector3 targetPositionP);
	void MoveStraight(Vector3 targetPositionP);
	void Attack(Figurines* targetP);
	void GetHit(int damageAmountP);

	/* Getters */
	const float GetMovementAction() { return m_CurrentMovementAction; }
	const int GetActionPoint() { return m_CurrentActionPoint; }
	const int GetHealthPoint() { return m_CurrentHealthPoint; }
	const float GetAttackRange() { return m_AttackRange; }
	const int getOwner() { return ownerID; }
	const bool IsDead() { return m_IsDead; }

private:
	/* References */
	GameEngine& m_GameEngine;

	bool m_IsSelected{ false };
	float m_UniformScale{ 0.02 };
	Vector3 m_Offset{ 0,0.f,0 };
	int ownerID{ 0 };

	/* Figurine Stats */
	float m_MaxMovementAction{ 108 };
	int m_MaxHealthPoint{ 2 };
	float m_AttackRange{ 50 };
	int m_MaxActionPoint{ 2 };

	/* Current Stats */
	int m_CurrentHealthPoint{ m_MaxHealthPoint };
	float m_CurrentMovementAction{ m_MaxMovementAction };
	int m_CurrentActionPoint{ m_MaxActionPoint };

	/* On selected Animation properties */
	float animationTime{ 0 };
	float scaleSpeed{ 2.f };
	float scaleFactor{ 0.002f };
	float flattenSpeed{ 1.f };
	float flattenFactor{ 0.001f };
	float m_MovementSpeed{ 10.f };

	/* On Movement */
	PathFindingComponent* pathfinding{ nullptr };
	std::vector<TurnThreshold*> m_Path;
	Vector3 straightTargetPosition;
	int m_IndexPosition{ 1 };
	bool m_IsMoving{ false };
	bool m_MoveStraight{ false };
	int turnSpeed{ 4 };

	/* On Death */
	bool m_IsDead = false;
	void LookAt(const Ogre::Vector3& targetPosition, float deltaTime, int turnSpeed);
};

