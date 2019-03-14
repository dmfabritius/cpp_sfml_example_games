#pragma once
#include "pch.h"
#include "GameSprite.h"
#include "GetReady.h"

class Ship : public GameSprite {
private:
    const float THRUST_AMOUNT = 0.05f;
    const float DAMPENING = 0.98f;
    const float MIN_VELOCITY = 0.01f;
    const float MIN_ACCELERATION = 0.01f;

    GetReady* getReady;
    float heading; // in degrees
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    bool thrusting; // flag to indicate if the ship is accelerating
    bool collision; // flag to indicate if an asteroid hit the ship
    sf::Clock clock; // used to control sprite animation

public:
    Ship() = delete; // prevent construction without a reference to the game
    Ship(Game* game);

    void enter(); // runs when the game state that this object belongs to becomes active (initialize)
    void update(float timeDelta);
    bool getCollision() const;
    void setCollision();

private:
    void updateHeading();
    void applyThrust();
};
