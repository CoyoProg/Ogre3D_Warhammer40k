#include "InputsManager.h"
#include <iostream>
#include "OgreInput.h"

bool InputsManager::keyPressed(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    if (key == SDLK_ESCAPE)
    {
        m_gameManager.QuitGame();
    }

    keysPressed.insert(key); // Add the pressed key to the set

    return true;
}

bool InputsManager::keyReleased(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    keysPressed.erase(key); // Remove the released key from the set

    return true;
}
