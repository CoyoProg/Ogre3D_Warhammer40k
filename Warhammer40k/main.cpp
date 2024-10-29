#include "GameManager.h"

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        GameManager app;
        app.initApp();
        app.getRoot()->startRendering();

        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
//! [fullsource]
