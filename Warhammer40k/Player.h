#pragma once
#include "Actors.h"

class Components;
class Figurines;

class Player : public Actors, public InputListener
{
public:
	Player(GameEngine& gameEngineP);
	~Player();

	virtual void Update(float deltaTime) override;

private:
	GameEngine& m_GameEngine;

	void OnLBMouseDown(int mouseX, int mouseY);
	void OnRBMouseDown(int mouseX, int mouseY);

	/* Mouse Ray */
	Ray mouseRay;
	Ray MouseRayTo3D(int mouseX, int mouseY);
	RaySceneQuery* m_RayScnQuery{ nullptr };

	/* Selected Actor */
	void SelectFigurine(Figurines* figurineP);
	void UnselectFigurine();
	bool m_IsActorSelected{ false };
	Figurines* m_CurrentSelected{ nullptr };

	float functionDelay = 0.1f;
	void IsTargetOnSight();
	bool m_OnSightFromSelected{ false };
	Vector3 m_CurrentSelectedPosition;
	Vector3 m_TargetPosition;
	Vector3 m_NewPosition;

	/* Inputs */
	virtual bool mouseMoved(const MouseMotionEvent& evt) override;
	virtual bool mousePressed(const MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const MouseButtonEvent& evt) override;
	bool m_IsLMBDown{ false };
	bool m_IsRMBDown{ false };
};

