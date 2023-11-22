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
	TurnThreshold(Vector2 pointOnLine, Vector2 pointPerdencicularToLine);

	bool HasCrossedLine(Vector2 point)
	{
		return GetSide(point) != approachSide;
	}

	void DrawLine(SceneManager* sceneMgr);
private:
	const float verticalLineGradient = 1e5f;

	float gradient{ 0 };
	float y_intercept{ 0 };
	Vector2 startPoint{ 0,0 };
	Vector2 targetPoint{ 0,0 };

	float gradientPerpendicular;
	bool approachSide{ false };

	bool GetSide(Vector2 point)
	{
		return (point.x - startPoint.x) * (targetPoint.y - startPoint.y) > 
			(point.y - startPoint.y) * (targetPoint.x - startPoint.x);
	}
};

