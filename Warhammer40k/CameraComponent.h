#pragma once
#include <OgreSceneQuery.h>

#include "Components.h"
#include "IListenerInputs.h"

class Figurines;

class CameraComponent : public Components, public IListenerInputs
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
	float m_ClampMinZ{ 55 };
	float m_ClampMaxZ{ 400 };
	Camera* m_Camera{ nullptr };
	SceneNode* m_CamNode{ nullptr };
	RaySceneQuery* m_RayScnQuery{ nullptr };

	/* Selected Actor */
	bool m_IsActorSelected{ false };
	Figurines* m_CurrentSelected{ nullptr };

	/* References */
	GameEngine& m_GameEngine;
	InputsManager& m_InputsManager;
	SceneManager& m_SceneManager;

	/* Inputs Listener*/
	void OnLBMouseDown(int mouseX, int mouseY) override;
	void OnKeyPressed(OgreBites::Keycode keyReleasedP) override;
	void OnKeyReleased(OgreBites::Keycode keyReleasedP) override;

	void UpdateCameraPosition(float deltaSec);
	void Zoom(float deltaTime);

	/* Game Mechanics */
	void SelectFigurine(Figurines* figurineP);
	void UnselectFigurine();
};

