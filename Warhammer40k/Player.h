#pragma once
#include "Actors.h"

class Components;
class Figurines;

namespace Ogre
{
	class OverlayManager;
}

/*
 *
 */
class Player : public Actors, public InputListener
{
public:
	Player(GameEngine &gameEngineP, Ogre::OverlayManager& overlayManagerP);
	~Player();

	virtual void Update(float deltaTimeP) override;
	void SwapPlayer() { mCurrentPlayerID = (mCurrentPlayerID == 1) ? 2 : 1; }
	int GetCurrentPlayer() const { return mCurrentPlayerID; }

private:
	void MouseRayTo3D(int mouseXP, int mouseYP);
	void CheckMouseOverCollisions();
	void HandleMouseOver(Figurines* mouseOverTargetP);
	void ResetMouseOver();

	void ShowFigurineCard(const Figurines &figurineP, bool isRightCardP = false);
	void SetCardTextValues(float movementPointP, int healthPointP, bool isRightCardP = false);
	void SelectFigurine(Figurines &figurineP);
	void UnselectFigurine();
	void HideCards();

	void OnLBMouseDown(int mouseXP, int mouseYP);
	void HandleFigurineSelection();

	void OnRBMouseDown(int mouseXP, int mouseYP);
	void CheckRightClickCollisions();
	void HandleFigurineTargeting(Ogre::RaySceneQueryResult::iterator& hitResult);
	void HandleFigurineMovement(Ogre::RaySceneQueryResult::iterator& hitResult);

	void UpdateCardText();

	virtual bool mouseMoved(const MouseMotionEvent& evt) override;
	virtual bool mousePressed(const MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const MouseButtonEvent& evt) override;

	Ogre::OverlayManager &m_OverlayManager;
	GameEngine &mGameEngine;

	RaySceneQuery *mRayScnQuery = nullptr;
	Ray mMouseRay;

	Entity *mDice = nullptr;

	int mCurrentPlayerID = 1;
	bool mIsFigurineSelected = false;
	Figurines *mCurrentSelected = nullptr;
	Figurines *mCurrentMouseOver = nullptr;

	bool mIsLMBDown = false;
	bool mIsRMBDown = false;
};

