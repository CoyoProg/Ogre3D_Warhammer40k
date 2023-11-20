#pragma once
#include "Actors.h"

class Figurines : public Actors
{
public:
	Figurines(SceneManager& sceneManagerP, std::string entityNameP, std::string nodeNameP);
	~Figurines();

	void Update(float deltaTime) override;
	void UpdatePositions(float deltaTime);

	Vector3 GetPosition() { return m_Node->getPosition(); }
	void SetPosition(Vector3 positionP);
	bool IsSleeping() { return !m_IsMoving; }

	void OnSelected(bool isSelected);
	void MoveTo(std::vector<Vector3> positionsP) { m_NextPositions = positionsP; m_IsMoving = true; }

	float GetMovementAction() { return m_MovementActionDistance; }
private:
	bool m_IsSelected{ false };
	float m_UniformScale{ 0.02 };
	Vector3 m_Offset{ 0,0.75f,0 };

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
	std::vector<Vector3> m_NextPositions;
	int m_IndexPosition{ 0 };
	bool m_IsMoving{ false };
};

