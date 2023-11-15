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
	Ray MouseRayTo3D(int mouseX, int mouseY);
	void OnRBMouseDown(int mouseX, int mouseY);
	RaySceneQuery* m_RayScnQuery{ nullptr };

	/* Selected Actor */
	bool m_IsActorSelected{ false };
	Figurines* m_CurrentSelected{ nullptr };

	void SelectFigurine(Figurines* figurineP);
	void UnselectFigurine();

	/* Inputs */
	virtual bool mousePressed(const MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const MouseButtonEvent& evt) override;
	bool m_IsLMBDown{ false };
	bool m_IsRMBDown{ false };
};

