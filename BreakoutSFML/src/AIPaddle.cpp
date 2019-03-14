#include "pch.h"
#include "Game.h"
#include "GameBall.h"
#include "AIPaddle.h"

AIPaddle::AIPaddle(Game* game, const float x, const float y) :
    PlayerPaddle(game, x, y, 0, 1500) {
}

void AIPaddle::updateVelocity() {
    float ballX = game->getBall()->getPosition().x;
    float thisX = getPosition().x;
    if (ballX - thisX < 10.0f && ballX - thisX > -10.0f)
        velocityX *= 0.4f;
    else if (ballX > thisX)
        velocityX += 40.0f;
    else
        velocityX -= 40.0f;
}