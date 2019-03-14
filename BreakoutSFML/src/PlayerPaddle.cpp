#include "pch.h"
#include "PlayerPaddle.h"
#include "Game.h"

PlayerPaddle::PlayerPaddle(Game* game, const float x, const float y, const float velocityX, const float maxVelocityX) :
    GameSprite(game, "images/paddle.png", 1, x, y),
    velocityX(velocityX),
    maxVelocityX(maxVelocityX) {
}

void PlayerPaddle::update(float elapsedTime) {
    updateVelocity();
    velocityX = std::min(std::max(-maxVelocityX, velocityX), maxVelocityX);
    elapsedTime = std::min(elapsedTime, 0.05f); // prevent huge elasped times when using the debugger
    sprite.move(velocityX * elapsedTime, 0);
    detectSideCollision();
}
void PlayerPaddle::updateVelocity() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocityX = (velocityX >= 0.0f) ? -150.0f : velocityX - 30.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocityX = (velocityX <= 0.0f) ? 150.0f : velocityX + 30.0f;
    } else { //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        velocityX = 0.0f;
    }
}
void PlayerPaddle::detectSideCollision() {
    sf::Vector2f pos = sprite.getPosition();
    float width = getWidth() / 2;
    if (pos.x < width) {
        sprite.setPosition(pos.x + 5, pos.y);
        velocityX = -velocityX; // Bounce in opposite direction
    }
    else if (pos.x > Game::SCREEN_WIDTH - width) {
        sprite.setPosition(pos.x - 5, pos.y);
        velocityX = -velocityX;
    }
}

float PlayerPaddle::getVelocity() const {
    return velocityX;
}
