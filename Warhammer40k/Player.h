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
	Player(GameEngine &gameEngineP);
	~Player();

	virtual void Update(float deltaTimeP) override;
	void SwapPlayer() { mPlayerID = (mPlayerID == 1) ? 2 : 1; }
	int GetCurrentPlayer() { return mPlayerID; }

private:
	/* References */
	Ogre::OverlayManager *m_OverlayManager = nullptr;
	GameEngine &mGameEngine;
	int mPlayerID = 1;

	void OnLBMouseDown(int mouseXP, int mouseYP);
	void OnRBMouseDown(int mouseXP, int mouseYP);

	void ShowFigurineCard(Figurines *figurineP, bool isRightCardP = false);
	void SetCardTextValues(float movementPointP, int healthPointP, bool isRightCardP = false);

	/* Mouse Ray */
	Ray mMouseRay;
	void MouseRayTo3D(int mouseXP, int mouseYP);
	void ResetMouseOver();
	RaySceneQuery *mRayScnQuery = nullptr;

	/* Dice */
	Entity *mDice;

	/* Selected Actor */
	void SelectFigurine(Figurines *figurineP);
	void UnselectFigurine();
	void HideCards();
	bool mIsActorSelected = false;
	Figurines *mCurrentSelected = nullptr;
	Figurines* mCurrentMouseOver = nullptr;

	float mFunctionDelay = 0.1f;
	void IsOnMovementSight();
	bool mOnSightFromSelected = false;
	float mDistanceFromSelected = 0.f;
	Vector3 mCurrentSelectedPosition;
	Vector3 mTargetPosition;
	Vector3 mNewPosition;

	/* Inputs */
	virtual bool mouseMoved(const MouseMotionEvent& evt) override;
	virtual bool mousePressed(const MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const MouseButtonEvent& evt) override;
	bool mIsLMBDown = false;
	bool mIsRMBDown = false;
};

