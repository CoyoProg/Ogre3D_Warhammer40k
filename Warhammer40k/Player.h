#pragma once
#include "Actors.h"

class Components;
class Figurines;

namespace Ogre
{
	class OverlayManager;
}

/*
 * @brief The Player class manages interactions for a player, handling figurine selection,
 * movement, targeting, and UI updates based on user input. It also facilitates turn
 * swapping between players, tracking the active player ID.
 */
class Player : public Actors, public InputListener
{
public:
	Player(GameEngine &gameEngineP, Ogre::OverlayManager& overlayManagerP);
	~Player();

	virtual void Update(float deltaTimeP) override;
	int GetCurrentPlayer() const { return mCurrentPlayerID; }

	virtual void OnEndTurn() override;

private:
	void MouseRayTo3D(int mouseXP, int mouseYP);
	void CheckMouseOverCollisions();
	void HandleMouseOver(Figurines* mouseOverTargetP);
	void ResetMouseOver();

	void OnLBMouseDown(int mouseXP, int mouseYP);
	void HandleFigurineSelection();
	void SelectPlayerFigurine(Figurines& figurineP);
	void UnselectFigurine();

	void OnRBMouseDown(int mouseXP, int mouseYP);
	void CheckRightClickCollisions();
	void HandleFigurineTargeting(Ogre::RaySceneQueryResult::iterator& hitResult);
	void HandleFigurineMovement(Ogre::RaySceneQueryResult::iterator& hitResult);

	void ShowFigurineCard(const Figurines& figurineP, bool isRightCardP = false);
	void SetCardTextValues(float movementPointP, int healthPointP, bool isRightCardP = false);
	void UpdateCardText(const Figurines& figurineP, bool isRightCardP);
	void HideCards();

	void SwapPlayer() { mCurrentPlayerID = (mCurrentPlayerID == 1) ? 2 : 1; }

	virtual bool mouseMoved(const MouseMotionEvent& evt) override;
	virtual bool mousePressed(const MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const MouseButtonEvent& evt) override;

	Ogre::OverlayManager &m_OverlayManager;
	GameEngine &mGameEngine;

	Camera *mCamera = nullptr;
	RaySceneQuery *mRayScnQuery = nullptr;
	Ray mMouseRay;

	Entity *mDice = nullptr;

	int mCurrentPlayerID = 1;
	bool mIsFigurineSelected = false;
	Figurines *mCurrentPlayerFigurine = nullptr;
	Figurines *mCurrentEnemyTargeted = nullptr;
	Figurines *mCurrentMouseOver = nullptr;

	bool mIsLMBDown = false;
	bool mIsRMBDown = false;
};

