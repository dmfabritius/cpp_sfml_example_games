#include "pch.h"
#include "Asteroid.h"
#include "ServiceManager.h"

Asteroid::Asteroid(Game* game, const std::string& filename) :
    GameSprite(game, filename),
    getReady(game->getPlayObject<GetReady>("GetReady")),
    ship(game->getPlayObject<Ship>("Ship")),
    spin(0.2f * Game::rand(-1, 1)),
    velocity(2.0f * Game::rand2f()) {
    velocity.x += 0.3f;
    velocity.y += 0.3f;
    do {
        setPosition(Game::randf(0, Game::SCREEN_WIDTH), Game::randf(0, Game::SCREEN_HEIGHT));
    } while (Game::distance2f(getPosition(), ship->getPosition()) < 250.0f);
}

void Asteroid::update(float timeDelta) {
    if (getReady->isActive()) return;   // wait for "get ready" message to complete before moving

    detectCollision();

    sprite.rotate(spin);
    sprite.move(velocity);
    wrap(velocity);
}

void Asteroid::detectCollision() {
    float distance = Game::distance2f(getPosition(), ship->getPosition());
    if (!ship->getCollision() && distance < (getWidth() + ship->getWidth()) * 0.5f) {
        ship->setCollision(); // the ship will respond to its collision
        ServiceManager::Audio()->playSound("audio/crash.wav");
    }
}
