#pragma once
#include "GameSprite.h"

class LaserPulse : public GameSprite {
private:
    const float PULSE_SPEED = 6;
    float elapsedTime;
    sf::Clock clock; // used to change sprite frames and animate the ball

public:
    LaserPulse() = delete;
    LaserPulse(Game* game, const sf::Vector2f& position, float heading);
    void update(float timeDelta);
    void setDestroyed();
};