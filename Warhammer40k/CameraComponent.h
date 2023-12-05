#pragma once
#include "Components.h"

class Figurines;

class CameraComponent : public Components, public InputListener
{
public:
	CameraComponent(GameEngine& gameEngineP);
	~CameraComponent();

	virtual void Update(float deltaTime) override;
	Camera* getCamera() { return m_Camera; }

private:
	/* Camera properties */
	float m_CameraSpeed{ 50.f };
	float m_MoveX{ 0.f };
	float m_MoveZ{ 0.f };
	float m_ZoomSpeed{ 20 };
	float m_ClampMinY{ 20 };
	float m_ClampMaxY{ 175 };
	float m_ClampMaxX{ 300 };
	float m_ClampMinZ{ -55 };
	float m_ClampMaxZ{ 300 };
	Camera* m_Camera{ nullptr };
	SceneNode* m_CamNode{ nullptr };

	/* Inputs Listener*/
	virtual bool mouseWheelRolled(const MouseWheelEvent& evt) override;
	virtual bool keyPressed(const KeyboardEvent& evt) override;
	virtual bool keyReleased(const KeyboardEvent& evt) override;
	virtual void frameRendered(const FrameEvent& evt) override;

	int m_MouseWheelY{ 0 };
	void UpdateCameraPosition(float deltaSec);
	void Zoom(float deltaTime);
};

