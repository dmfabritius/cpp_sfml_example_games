#include "pch.h"
#include "Game.h"
#include "GameBall.h"
#include "AIPaddle.h"

AIPaddle::AIPaddle(Game* game, const float x, const float y) :
    PlayerPaddle(game, x, y, 0, 700) {
}

void AIPaddle::updateVelocity() {
    static const GameBall* ball = game->getPlayObject<GameBall>("Ball");
    float ballX = ball->getPosition().x;
    float thisX = getPosition().x;
    if (ballX - thisX < 10.0f && ballX - thisX > -10.0f)
        velocity *= 0.4f;
    else if (ballX > thisX)
        velocity += 40.0f;
    else
        velocity -= 40.0f;
}