#include "pch.h"
#include "Brick.h"
#include "Game.h"
#include "ServiceManager.h"


Brick::Brick(Game* game, const float x, const float y, const sf::Color& color, int points) :
    GameSprite(game, "images/brick.png", 1, x, y),
    points(points) {
    sprite.setColor(color);
}

void Brick::update(float elapsedTime) {
    if (game->getBall()->getGlobalBounds().intersects(this->getGlobalBounds())) {
        active = false;
        game->updateScore(points);
        game->getBall()->fixOverlap(this);
        game->getBall()->reflectAngle(); // bounce off the paddle
        ServiceManager::Audio()->playSound("audio/blip.wav");
        destroyed = true;
    }
}
