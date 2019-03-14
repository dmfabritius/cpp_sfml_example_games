#pragma once
#include "PlayerPaddle.h"

class AIPaddle : public PlayerPaddle {
public:
    AIPaddle() = delete; // prevent construction without initial position
    AIPaddle(Game* game, const float x, const float y);

    void updateVelocity();
};