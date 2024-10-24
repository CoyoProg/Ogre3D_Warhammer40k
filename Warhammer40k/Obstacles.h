#pragma once
#include "Actors.h"

class GameEngine;
class Grid;

/*
 * @brief Obstacles are objects or buildings that block one or more tiles,
 * preventing figurines from walking over them.
 */
class Obstacles : public Actors
{
public:
	/* The scale always needs to be positive. */
	Obstacles(GameEngine &gameEngineP, const Vector2 &gridCoordsP, const Vector3& scaleP, const std::string &ID);
	~Obstacles();

	void UpdateTilesCollision(int coordXP, int coordYP, bool isFlippedP);

	/**
	 * @brief Flips the tile collisions of this obstacle, mirroring them across the X and Y axes.
	 *
	 * This method updates the collision states of the tiles to match the new position of the obstacle,
	 * effectively reversing their direction.
	 */
	void FlipCollisions();

private:
	void CreateCube(const std::string& ID);
	void SetCubePosition();

	Grid &mGrid;
	SceneManager& mSceneManager;

	bool mIsFlipped = false;
	Vector3 mScale;
	Vector2 mGridCoords;
};

