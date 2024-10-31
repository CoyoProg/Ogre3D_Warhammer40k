#include "TurnThreshold.h"
#include "Ogre.h"
#include <iostream>

TurnThreshold::TurnThreshold(Vector2 pointOnLineP, Vector2 pointPerdencicularToLineP)
{
	float deltaX = pointOnLineP.x - pointPerdencicularToLineP.x;
	float deltaY = pointOnLineP.y - pointPerdencicularToLineP.y;

	if (deltaX == 0)
	{
		mGradientPerpendicular = mVerticalLineGradient;
	}
	else
	{
		mGradientPerpendicular = deltaY / deltaX;
	}

	if (mGradientPerpendicular == 0)
	{
		mGradient = mVerticalLineGradient;
	}
	else
	{
		mGradient = -1 / mGradientPerpendicular;
	}

	mY_Intercept = pointOnLineP.y - mGradient * pointOnLineP.x;
	mStartPoint = pointOnLineP;
	mTargetPoint = pointOnLineP + Vector2(1, mGradient);

	mIsNearSide = GetSide(pointPerdencicularToLineP);
}

void TurnThreshold::DrawLine(SceneManager *sceneMgrP)
{
	ManualObject *manualObject = sceneMgrP->createManualObject();
	manualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);

	Vector3 lineDir(1, 0, mGradient);
	Vector3 lineStart = Vector3(mStartPoint.x, 0.12f, mStartPoint.y);

	/* Normalize the direction vector */
	Vector3 normalizedLineDir = lineDir.normalisedCopy();

	/* Define a distance for the line */
	float lineLength = 3.f;

	/* Calculate the end point of the line */
	Vector3 lineEnd = lineStart + normalizedLineDir * lineLength / 2;
	lineStart = lineStart - normalizedLineDir * lineLength / 2;

	/* Define the vertices of the line */
	manualObject->position(lineStart);
	manualObject->position(Vector3(lineEnd.x, 0.12f, lineEnd.z));

	manualObject->end();

	/* Create a scene node to attach the manual object */
	Ogre::SceneNode *node = sceneMgrP->getRootSceneNode()->createChildSceneNode();
	node->attachObject(manualObject);
}
