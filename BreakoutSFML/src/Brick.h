#pragma once
#include "GameSprite.h"
#include "GameBall.h"

class Brick : public GameSprite {
private:
    int points;

public:
    Brick() = delete; // prevent construction without initial position
    Brick(Game* game, const float x, const float y, const sf::Color& color, int points);

    void update(const float elapsedTime);
};