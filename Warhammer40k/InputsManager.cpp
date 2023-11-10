#include "InputsManager.h"
#include <iostream>

bool InputsManager::keyPressed(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    if (key == SDLK_ESCAPE)
    {
        m_GameManager.QuitGame();
    }

    for (auto& listerners : m_MyListeners)
    {
        listerners->OnKeyPressed(key);
    }

    return true;
}

bool InputsManager::keyReleased(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    for (auto& listerners : m_MyListeners)
    {
        listerners->OnKeyReleased(key);
    }

    return true;
}

bool InputsManager::mouseWheelRolled(const MouseWheelEvent& evt)
{
    m_MouseWheelY = evt.y;

    if (evt.y != 0)
    {
        return true;
    }else
        return false;
}

bool InputsManager::mousePressed(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        for (auto& listerners : m_MyListeners)
        {
            listerners->OnLBMouseDown(evt.x, evt.y);
        }

        m_IsLMBDown = true;
    }
    
    if (key == 3)
    {
        m_IsRMBDown = true;
    }

    return false;
}

bool InputsManager::mouseReleased(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        m_IsLMBDown = false;
    }
    
    if (key == 3)
    {
        m_IsRMBDown = false;
    }

    return false;
}

void InputsManager::frameRendered(const Ogre::FrameEvent& evt)
{
    m_MouseWheelY = 0;
}
