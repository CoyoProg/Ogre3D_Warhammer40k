#pragma once
#include "Actors.h"

class Figurines : public Actors
{
public:
	Figurines(SceneManager& sceneManagerP, std::string entityNameP, std::string nodeNameP);
	~Figurines();

	void Update(float deltaTime) override;

	void SetPosition(Vector3 positionP);
	void OnSelected(bool isSelected);

private:
	bool m_IsSelected{ false };
	float uniformScale{ 0.02 };

	/* On selected Animation properties */
	float animationTime{ 0 };
	float scaleSpeed{ 2.f };
	float scaleFactor{ 0.001f };
	float flattenSpeed{ 1.f };
	float flattenFactor{ 0.0005f };
};

