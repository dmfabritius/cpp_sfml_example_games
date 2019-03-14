#include "pch.h"
#include "LaserPulse.h"

LaserPulse::LaserPulse(Game* game, const sf::Vector2f& position, float heading) :
    GameSprite(game, "images/laserpulse.png", 4),
    elapsedTime(0) {
    entity.setOrigin(getWidth() * 0.5f, getHeight() * 0.5f);
    entity.rotate(heading);
    velocity = PULSE_SPEED * Game::angleToVector2f(heading);
    setPosition(position);
}

void LaserPulse::update(float timeDelta) {
    elapsedTime += timeDelta;
    if (elapsedTime > 2.0f) { // pulses get destroyed after 2 seconds, unless something happens to them before that
        destroyed = true;
        game->removeLaserPulse(this);
    } else {
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            frameIndex = 1 + (frameIndex + 1) % (numFrames - 1); // animate the ship thrusters
            clock.restart();
        }
        entity.move(velocity); // update the pulse's position
        wrap();
    }
}

void LaserPulse::setDestroyed() {
    destroyed = true;
}
