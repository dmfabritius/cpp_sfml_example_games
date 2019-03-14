#pragma once
#include "GameSprite.h"
#include "GetReady.h"

class Ship : public GameSprite {
private:
    const float THRUST_AMOUNT = 0.05f;
    const float DAMPENING = 0.98f;
    const float MIN_VELOCITY = 0.01f;
    const float MIN_ACCELERATION = 0.01f;
    const unsigned int MAX_LASER_PULSES = 6;

    GetReady* getReady;
    bool collision;
    bool thrusting;
    sf::Vector2f startingPosition;
    float heading; // in degrees
    sf::Clock clock; // used to change sprite frames and animate the ball

public:
    Ship() = delete;
    Ship(Game* game);
    void enter();
    void update(float timeDelta);
    void fireLaserPulse();
    bool getCollision() const;
    void setCollision();

private:
    void updateHeading();
    void applyThrust();
};
