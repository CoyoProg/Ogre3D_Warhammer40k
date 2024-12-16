#pragma once
#include "Actors.h"

struct FlipTableAnimation
{
	static constexpr float bumpHeight = 10.0f; // Height of the bump in units

	float animationProgress = 0.0f; // Progress of the flip animation
	float animationSpeed = 0.f;
	float rotationAngle = 0;

	bool isFlipping = false;
	bool shouldChangeDir = false;
	int flipFlop = 1;
};

/*
 * @brief Represents the game’s tabletop area, acting as the main surface for game elements.
 */
class TableTop : public Actors
{
public:
	TableTop(GameManager &gameManagerP);

	virtual void Update(float deltaTimeP) override;
	virtual void OnEndTurn() override;

private:
	void FlipTableTop(float deltaTimeP);

	void ResetAnimationProperties();
	/* Snap the tabletop to the right position and rotation at the end of the animation */
	void ResetTableTransform();

	/* Flip table Rotation Animation */
	void UpdateRotationEffect();
	/* Flip table Bump Animation */
	void UpdateBumpEffect();
	/* Flip table Animation Progress*/
	void UpdateAnimationProgress(float deltaTimeP);

	GameManager& mGameManager;
	FlipTableAnimation mFlipAnimation;
};

