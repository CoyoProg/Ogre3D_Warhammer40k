#pragma once
#include "Actors.h"

class Components;
class Figurines;

namespace Ogre
{
	class OverlayManager;
}

class Player : public Actors, public InputListener
{
public:
	Player(GameEngine& gameEngineP);
	~Player();

	virtual void Update(float deltaTime) override;
	void SwapPlayer() { PlayerID = (PlayerID == 1) ? 2 : 1; }
	int GetCurrentPlayer() { return PlayerID; }

private:
	/* References */
	Ogre::OverlayManager* m_OverlayManager;
	GameEngine& m_GameEngine;
	int PlayerID{ 1 };

	void OnLBMouseDown(int mouseX, int mouseY);
	void OnRBMouseDown(int mouseX, int mouseY);

	void SetCardTextValues(float movementPoint, int healthPoint, bool isRightCard = false);

	/* Mouse Ray */
	Ray mouseRay;
	Ray MouseRayTo3D(int mouseX, int mouseY);
	RaySceneQuery* m_RayScnQuery{ nullptr };

	/* Dice */
	Entity* m_Dice;

	/* Selected Actor */
	void SelectFigurine(Figurines* figurineP);
	void UnselectFigurine();
	bool m_IsActorSelected{ false };
	Figurines* m_CurrentSelected{ nullptr };

	float functionDelay = 0.1f;
	void IsOnMovementSight();
	bool m_OnSightFromSelected{ false };
	float distanceFromSelected = 0;
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

