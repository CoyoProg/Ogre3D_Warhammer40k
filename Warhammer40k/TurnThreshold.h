#pragma once
#include <OgreVector2.h>

using namespace Ogre;

/*  With A starting point and B the target point
	We use the perpendicular line from the Gradient AB at the threshold distance point
	to know when a position bypass that point.
	Code taken from Sebastien Lague https://www.youtube.com/watch?v=NjQjl-ZBXoY */

struct TurnThreshold
{
public:
	TurnThreshold(Vector2 pointOnLineP, Vector2 pointPerdencicularToLineP);

	bool HasCrossedLine(Vector2 pointP)
	{
		return GetSide(pointP) != mIsNearSide;
	}

	void DrawLine(SceneManager *sceneMgrP);

private:
	const float mVerticalLineGradient = 1e5f;

	float mGradient = 0;
	float mY_Intercept = 0;
	Vector2 mStartPoint = Vector2(0);
	Vector2 mTargetPoint = Vector2(0);

	float mGradientPerpendicular = 0;
	bool mIsNearSide = false;

	bool GetSide(Vector2 pointP)
	{
		return (pointP.x - mStartPoint.x) * (mTargetPoint.y - mStartPoint.y) >
			(pointP.y - mStartPoint.y) * (mTargetPoint.x - mStartPoint.x);
	}
};

