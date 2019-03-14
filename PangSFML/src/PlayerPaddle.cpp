#include "pch.h"
#include "PlayerPaddle.h"
#include "Game.h"

PlayerPaddle::PlayerPaddle(Game* game, const float x, const float y, const float velocity, const float maxVelocity) :
    GameSprite(game, "images/paddle.png", 1, x, y),
    velocity(velocity),
    maxVelocity(maxVelocity) {
    sprite.setOrigin(
        sprite.getGlobalBounds().width / numFrames / 2,
        sprite.getGlobalBounds().height / numFrames / 2);
}

void PlayerPaddle::update(float elapsedTime) {
    updateVelocity();
    velocity = std::min(std::max(-maxVelocity, velocity), maxVelocity);
    elapsedTime = std::min(elapsedTime, 0.05f); // prevent huge elasped times when using the debugger
    sprite.move(velocity * elapsedTime, 0);
    detectSideCollision();
}
void PlayerPaddle::updateVelocity() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity = (velocity >= 0.0f) ? -150.0f : velocity - 30.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity = (velocity <= 0.0f) ? 150.0f : velocity + 30.0f;
    } else { //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        velocity = 0.0f;
    }
}
void PlayerPaddle::detectSideCollision() {
    sf::Vector2f pos = sprite.getPosition();
    float width = getWidth() / 2;
    if (pos.x < width) {
        sprite.setPosition(pos.x + 5, pos.y);
        velocity = -velocity; // Bounce by current velocity in opposite direction
    }
    else if (pos.x > Game::SCREEN_WIDTH - width) {
        sprite.setPosition(pos.x - 5, pos.y);
        velocity = -velocity;
    }
}

float PlayerPaddle::getVelocity() const {
    return velocity;
}
int PlayerPaddle::getScore() const {
    return score;
}
int PlayerPaddle::resetScore() {
    return (score = 0);
}
void PlayerPaddle::addPoints(const int points) {
    this->score += points;
}
