#pragma once
#include "OgreInput.h"

class IListenerInputs
{
public:
	virtual void OnLBMouseDown(int mouseXP, int mouseYP) = 0;
	virtual void OnKeyPressed(OgreBites::Keycode keyReleasedP) = 0;
	virtual void OnKeyReleased(OgreBites::Keycode keyPressedP) = 0;
};