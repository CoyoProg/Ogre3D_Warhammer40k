#include "TurnThreshold.h"
#include "Ogre.h"
#include <iostream>

TurnThreshold::TurnThreshold(Vector2 pointOnLine, Vector2 pointPerdencicularToLine)
{
	float deltaX = pointOnLine.x - pointPerdencicularToLine.x;
	float deltaY = pointOnLine.y - pointPerdencicularToLine.y;

	if (deltaX == 0)
	{
		gradientPerpendicular = verticalLineGradient;
	}
	else
	{
		gradientPerpendicular = deltaY / deltaX;
	}

	if (gradientPerpendicular == 0)
	{
		gradient = verticalLineGradient;
	}
	else
	{
		gradient = -1 / gradientPerpendicular;
	}

	y_intercept = pointOnLine.y - gradient * pointOnLine.x;
	startPoint = pointOnLine;
	targetPoint = pointOnLine + Vector2(1, gradient);

	approachSide = GetSide(pointPerdencicularToLine);
}

void TurnThreshold::DrawLine(SceneManager* sceneMgr)
{
	ManualObject* manualObject = sceneMgr->createManualObject();
	manualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);

	Vector3 lineDir(1, 0, gradient);
	Vector3 lineStart = Vector3(startPoint.x, 0.12f, startPoint.y);

	// Normalize the direction vector
	Vector3 normalizedLineDir = lineDir.normalisedCopy();

	// Define a distance for the line
	float lineLength = 3.f;

	// Calculate the end point of the line
	Vector3 lineEnd = lineStart + normalizedLineDir * lineLength / 2;
	lineStart = lineStart - normalizedLineDir * lineLength / 2;

	// Define the vertices of the line
	manualObject->position(lineStart);
	manualObject->position(Vector3(lineEnd.x, 0.12f, lineEnd.z));

	manualObject->end();

	// Create a scene node to attach the manual object
	Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(manualObject);
}
