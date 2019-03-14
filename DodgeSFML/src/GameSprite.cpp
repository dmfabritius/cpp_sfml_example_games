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
        sprite.setOrigin(getWidth() * 0.5f, getHeight() * 0.5f);
        sprite.setPosition(x, y);
        int frameWidth = image.getSize().x / numFrames;
        int frameHeight = image.getSize().y;
        for (int i = 0; i < numFrames; i++) {
            // - our sprite images will contain one or more frames in a horizontal row
            // - create a bounding box around each frame of the sprite's image
            //   moving the left edge across the image for each frame
            frames.push_back(sf::IntRect(frameWidth * i, 0, frameWidth, frameHeight));
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
    return sprite.getGlobalBounds().left;
}
float GameSprite::getRight() const {
    return sprite.getGlobalBounds().left + getWidth();
}
float GameSprite::getTop() const {
    return sprite.getGlobalBounds().top;
}
float GameSprite::getBottom() const {
    return sprite.getGlobalBounds().top + getHeight();
}
sf::FloatRect GameSprite::getGlobalBounds() const {
    return sprite.getGlobalBounds();
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

// to work correctly, the origin of the entity must be its center
void GameSprite::wrap(const sf::Vector2f& velocity) {
    float x = getPosition().x;
    float y = getPosition().y;;
    float w = 0.5f * getWidth();
    float h = 0.5f * getHeight();
    if (x < -w && velocity.x < 0)
        x = Game::SCREEN_WIDTH + w;
    else if (x > Game::SCREEN_WIDTH + w && velocity.x > 0)
        x = -w;
    if (y < -h && velocity.y < 0)
        y = Game::SCREEN_HEIGHT + h;
    else if (y > Game::SCREEN_HEIGHT + h && velocity.y > 0)
        y = -h;
    setPosition(x, y);
}
