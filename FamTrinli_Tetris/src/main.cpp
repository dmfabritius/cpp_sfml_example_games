#include "pch.h"
#include "Game.h"

// base on the video "Let's make 16 games in C++: TETRIS" by FamTrinli
// https://www.youtube.com/watch?v=zH_omFPqMO4
//
int main(int argc, char* argv[]) {

    srand((unsigned int)time(0)); // seed random number generator

    Game app;
    app.run();

    return EXIT_SUCCESS;
}
