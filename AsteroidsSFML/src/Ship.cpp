#include "pch.h"
#include "Ship.h"

// https://opengameart.org/content/2d-spaceship-sprites-with-engines
Ship::Ship(Game* game) :
    GameSprite(game, "images/ship.png", 8),
    getReady(game->getPlayObject<GetReady>("GetReady")) {
    entity.setOrigin(getWidth() * 0.5f, getHeight() * 0.5f);
    startingPosition.x = Game::SCREEN_WIDTH * 0.5f;
    startingPosition.y = Game::SCREEN_HEIGHT * 0.5f;
    enter();
}

void Ship::enter() {
    setPosition(startingPosition);
    velocity = sf::Vector2f(0, 0);
    acceleration = sf::Vector2f(0, 0);
    collision = false;
    thrusting = false;
    heading = 0;
    frameIndex = 0;
}

void Ship::update(float timeDelta) {
    if (getReady->isActive()) return;
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
    entity.setRotation(heading);
    entity.move(velocity); // update the ship's position
    wrap();
}

void Ship::updateHeading() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        heading -= 3.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        heading += 3.0f;
    }
}

void Ship::applyThrust() {
    thrusting = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    if (thrusting) {
        game->getAudio().playSound("audio/thrust.wav");
        acceleration = THRUST_AMOUNT * Game::angleToVector2f(heading);
    } else {
        // continually slow down when not actively thrusting, but don't stop completely
        if (std::abs(Game::magnitude2f(acceleration)) > MIN_ACCELERATION) acceleration *= DAMPENING;
        if (std::abs(Game::magnitude2f(velocity)) > MIN_VELOCITY) velocity *= DAMPENING;
    }
    velocity += acceleration;
}

void Ship::fireLaserPulse() {
    if (!getReady->isActive() && game->getNumLaserPulses() < MAX_LASER_PULSES) {
        game->getAudio().playSound("audio/fire.wav", true);
        game->addLaserPulse(entity.getPosition(), heading);
    }
}

bool Ship::getCollision() const {
    return collision;
}

void Ship::setCollision() {
    collision = true;
}
