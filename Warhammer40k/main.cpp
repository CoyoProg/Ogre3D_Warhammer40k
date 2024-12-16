//#include "GameManager.h"
#include "GameEngine.h"

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        GameEngine engine;
        engine.initApp();
        engine.getRoot()->startRendering();

        engine.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
