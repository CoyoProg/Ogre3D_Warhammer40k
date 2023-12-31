#pragma once
#include "Actors.h"
#include "TurnThreshold.h"

class PathFindingComponent;

class Figurines : public Actors
{
public:
	Figurines(GameEngine& gameEngineP, std::string entityNameP, std::string nodeNameP, int owner);
	~Figurines();

	void Update(float deltaTime) override;
	void UpdatePositions(float deltaTime);

	Vector3 GetPosition() { return m_Node->_getDerivedPosition(); }
	void SetPosition(Vector3 positionP);
	void SetYawRotation(Degree rotationP);
	bool IsSleeping() { return !m_IsMoving; }

	void OnSelected(bool isSelected);

	void MoveTo(Vector3 targetPositionP);
	float GetMovementAction() { return m_MovementActionDistance; }
	int getOwner() { return ownerID; }

private:
	bool m_IsSelected{ false };
	float m_UniformScale{ 0.02 };
	Vector3 m_Offset{ 0,0.75f,0 };
	int ownerID{ 0 };

	/* Figurine Stats */
	float m_MovementActionDistance{50};
	float m_MovementSpeed{ 10.f };

	/* On selected Animation properties */
	float animationTime{ 0 };
	float scaleSpeed{ 2.f };
	float scaleFactor{ 0.001f };
	float flattenSpeed{ 1.f };
	float flattenFactor{ 0.0005f };

	/* On Movement */
	PathFindingComponent* pathfinding{ nullptr };
	std::vector<TurnThreshold*> m_Path;
	int m_IndexPosition{ 1 };
	bool m_IsMoving{ false };
	int turnSpeed{ 4 };

	void LookAt(const Ogre::Vector3& targetPosition, float deltaTime, int turnSpeed);
};

