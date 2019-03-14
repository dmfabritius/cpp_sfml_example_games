#pragma once
#include "GameEntity.h"
#include "GetReady.h"
#include "Ship.h"

#define ASTEROID_LARGE_SIZE 70.0f
#define ASTEROID_MEDIUM_SIZE 38.0f

class Asteroid : public GameEntity<sf::ConvexShape> {
private:
    GetReady* getReady;
    Ship* ship;
    int numVertices;
    float radius;
    float spin;

public:
    Asteroid() = delete;
    Asteroid(Game* game,
             const float radius = ASTEROID_LARGE_SIZE,
             const sf::Vector2f& position = sf::Vector2f(0, 0));
    void update(float timeDelta);

private:
    void createShape();
    void setVelocity();
    void detectCollision();
    void collide(bool addPoints);
    void laserPulseCollision();
};
