#pragma once
#include "pch.h"
#include "GameSprite.h"
#include "GetReady.h"
#include "Ship.h"

class Asteroid : public GameSprite {
private:
    GetReady* getReady;
    Ship* ship;
    float spin;
    sf::Vector2f velocity;

public:
    Asteroid() = delete; // prevent construction without a reference to the game
    Asteroid(Game* game, const std::string& filename);

    void update(float timeDelta);

private:
    void detectCollision(); // determines if this asteroid hit the ship
};
