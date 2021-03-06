#include "pch.h"
#include "Game.h"

// Based on the tutorial available at:
// https://www.gamefromscratch.com/page/Game-From-Scratch-CPP-Edition.aspx
//
// updated to use SFML v2.5.1 and Visual C++ 15 (Visual Studio 2017)
//
int main(int argc, char* argv[]) {
    srand((unsigned int)time(0)); // seed random number generator

    Game app;
    app.run();

    return EXIT_SUCCESS;
}
