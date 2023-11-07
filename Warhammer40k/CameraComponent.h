#pragma once
#include "Components.h"
#include "InputsManager.h"

class CameraComponent : public Components
{
public:
	CameraComponent(SceneManager& sceneManagerP, const InputsManager& inputsManagerP);
	~CameraComponent() {};

	virtual void Update(float deltaTime) override;
	Camera* getCamera() { return m_Camera; }

private:
	Camera* m_Camera;
	SceneNode* m_CamNode;

	const InputsManager& inputManager;

	void updateCameraPosition(float deltaSec);
};

