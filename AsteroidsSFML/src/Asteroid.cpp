#include "pch.h"
#include "Asteroid.h"
#include "LaserPulse.h"

Asteroid::Asteroid(Game* game, const float radius, const sf::Vector2f& position) :
    GameEntity(game),
    getReady(game->getPlayObject<GetReady>("GetReady")),
    ship(game->getPlayObject<Ship>("Ship")),
    numVertices(Game::rand(9, 14)),
    radius(radius * Game::rand(100, 133) * 0.01f), // randomly increase size up to 133%
    spin(0.2f * Game::rand(-1, 1)) {
    if (position.x == 0) // if not given a position, start in a random spot at least 250 pixels from the ship
        do {
            entity.setPosition(Game::randf(0, Game::SCREEN_WIDTH), Game::randf(0, Game::SCREEN_HEIGHT));
        } while (Game::distance2f(getPosition(), ship->getPosition()) < 250.0f);
    else
        entity.setPosition(position);
    entity.setFillColor(sf::Color::Transparent);
    entity.setOutlineThickness(1);
    entity.setOutlineColor(sf::Color::White);
    entity.setPointCount(numVertices);
    createShape();
    setVelocity();
}

void Asteroid::update(float timeDelta) {
    if (getReady->isActive()) return;
    detectCollision();
    entity.rotate(spin);
    entity.move(velocity);
    wrap();
}

void Asteroid::createShape() {
    for (int i = 0; i < numVertices; i++) {
        float a = TWO_PI * static_cast<float>(i) / static_cast<float>(numVertices);
        float r = radius * Game::rand(75, 125) * 0.01f; // shift each vertex in or out to make it jagged
        float x = r * std::cos(a);
        float y = r * std::sin(a);
        entity.setPoint(i, { x,y });
    }
}

void Asteroid::setVelocity() {
    velocity = Game::rand2f();
    // set minimum speed
    velocity.x = std::min(0.3f, velocity.x);
    velocity.y = std::min(0.3f, velocity.y);
    // adjust the speed inversely with its size
    float scaleFactor =
        (radius > ASTEROID_LARGE_SIZE) ? 1.5f
        : (radius > ASTEROID_MEDIUM_SIZE) ? 2.5f
        : 3.5f;
    velocity *= scaleFactor;
}

void Asteroid::detectCollision() {
    float distance;
    distance = Game::distance2f(getPosition(), ship->getPosition());
    if (!ship->getCollision() && 0.96f * distance < radius + ship->getWidth() * 0.5f) {
        ship->setCollision(); // the ship will handle its own collision
        collide(false);
    } else {
        auto it = game->getLaserPulses().begin();
        while (it != game->getLaserPulses().end()) { // loop thru all the laser pulses
            LaserPulse* pulse = *it; // dereference the iterator pointer to get a LaserPulse pointer
            distance = Game::distance2f(getPosition(), pulse->getPosition());
            if (distance < radius) { // size of laser pulse is neglible, so not included
                collide(true);
                laserPulseCollision();
                pulse->setDestroyed();
                it = game->getLaserPulses().erase(it); // remove it from the collection of pulses & advance to next
            } else {
                it++;
            }
        }
    }
}

void Asteroid::collide(bool addPoints) {
    if (radius > ASTEROID_LARGE_SIZE) {
        game->getAudio().playSound("audio/bangLarge.wav", true);
        if (addPoints) game->updateScore(10);
    } else if (radius > ASTEROID_MEDIUM_SIZE) {
        game->getAudio().playSound("audio/bangMedium.wav", true);
        if (addPoints) game->updateScore(20);
    } else {
        game->getAudio().playSound("audio/bangSmall.wav", true);
        if (addPoints) game->updateScore(40);
    }
}

void Asteroid::laserPulseCollision() {
    if (radius >= ASTEROID_MEDIUM_SIZE) { // large and medium asteroids break apart
        radius *= 0.55f;
        spin *= 1.5f;
        createShape();
        setVelocity();
        game->addAsteroid(radius, getPosition());
    } else {
        destroyed = true; // small asteroids get destroyed
        game->removeAsteroid();
        if (game->getNumAsteroids() < 4) game->createAsteroids(1);
    }
}
