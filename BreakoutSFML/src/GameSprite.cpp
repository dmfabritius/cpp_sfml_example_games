#include "pch.h"
#include "Game.h"
#include "GameSprite.h"

GameSprite::GameSprite(
    Game* game,
    const std::string& filename,
    const int numFrames,
    const float x, const float y) :
    GameObject(game),
    numFrames(numFrames),
    frameIndex(0) {
    if (image.loadFromFile(filename)) {
        sprite.setTexture(image);
        sprite.setOrigin(getWidth() * 0.5f, getHeight() * 0.5f); // set the origin point for every sprite to be its center
        sprite.setPosition(x, y);
        for (int i = 0; i < numFrames; i++) {
            // - our sprite images will contain one or more frames in a horizontal row
            // - create a bounding box around each frame of the sprite's image
            //   moving the left edge across the image for each frame
            frames.push_back(sf::IntRect((int)getWidth() * i, 0, (int)getWidth(), (int)getHeight()));
        }
    } else {
        throw std::runtime_error(filename + " was not found in GameSprite::GameSprite");
    }
}

void GameSprite::enter() {
    // generic game sprites don't have any set-up/initialization actions
    // objects derived from this class can implement their own
}
void GameSprite::leave() {
    // generic game sprites don't have any clean-up actions
    // objects derived from this class can implement their own
}

void GameSprite::update(float timeDelta) {
        // generic game sprites don't have any defined animation
        // objects derived from this class can implement their own
}
void GameSprite::draw() {
    if (active) {
        sprite.setTextureRect(frames[frameIndex]);
        game->getWindow().draw(sprite);
    }
}

float GameSprite::getWidth() const {
    return sprite.getGlobalBounds().width / numFrames;
}

float GameSprite::getHeight() const {
    return sprite.getGlobalBounds().height;
}

float GameSprite::getLeft() const {
    return sprite.getPosition().x - getWidth() * 0.5f;
}
float GameSprite::getRight() const {
    return sprite.getPosition().x + getWidth() * 0.5f;
}
float GameSprite::getTop() const {
    return sprite.getPosition().y - getHeight() * 0.5f;
}
float GameSprite::getBottom() const {
    return sprite.getPosition().y + getHeight() * 0.5f;
}

sf::FloatRect GameSprite::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::FloatRect GameSprite::getLocalBounds() const {
    return sprite.getLocalBounds();
}

sf::Vector2f GameSprite::getPosition() const {
    return sprite.getPosition();
}

void GameSprite::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

void GameSprite::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}
