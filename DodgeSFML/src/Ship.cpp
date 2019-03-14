#include "pch.h"
#include "Ship.h"
#include "ServiceManager.h"

Ship::Ship(Game* game) :
    GameSprite(game, "images/ship.png", 8),
    getReady(game->getPlayObject<GetReady>("GetReady")) {
    setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
}

void Ship::enter() {
    setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
    heading = 0;
    velocity = sf::Vector2f(0, 0);
    acceleration = sf::Vector2f(0, 0);
    thrusting = false;
    collision = false;
    frameIndex = 0;
}

void Ship::update(float timeDelta) {
    if (getReady->isActive()) return;   // wait for "get ready" message to complete before moving
    updateHeading();
    applyThrust();

    if (thrusting) {
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            clock.restart();
            frameIndex = 1 + (frameIndex + 1) % 3; // animate the ship thrusters
        }
    } else if (collision) {
        if (clock.getElapsedTime().asMilliseconds() > 200) {
            clock.restart();
            if (frameIndex < 3) {
                frameIndex = 3;
            }
            if (frameIndex != 7) ++frameIndex; // animate ship explosion
            if (frameIndex == 7) game->setStateGameOver();
        }
    } else {
        frameIndex = 0; // ship with no thrust
    }

    game->updateScore(1); // gain one point for each frame/update cycle

    sprite.setRotation(heading);
    sprite.move(velocity);
    wrap(velocity);
}

bool Ship::getCollision() const {
    return collision;
}

void Ship::setCollision() {
    collision = true;
}

void Ship::updateHeading() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        heading -= 3.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        heading += 3.0f;
    }
}

void Ship::applyThrust() {
    thrusting =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    if (thrusting) {
        ServiceManager::Audio()->playSound("audio/thrust.wav");
        acceleration = THRUST_AMOUNT * Game::angleToVector2f(heading);
    } else {
        // continually slow down when not actively thrusting, but don't stop completely
        if (std::abs(Game::magnitude2f(acceleration)) > MIN_ACCELERATION) acceleration *= DAMPENING;
        if (std::abs(Game::magnitude2f(velocity)) > MIN_VELOCITY) velocity *= DAMPENING;
    }
    velocity += acceleration;
}

