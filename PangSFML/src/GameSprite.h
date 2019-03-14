#pragma once
#include "pch.h"
#include "GameObject.h"

class GameSprite : public GameObject {
protected:
    sf::Sprite sprite;
    sf::Texture image;
    int numFrames; // sprite images can be composites of multiple frames in a horizontal row
    int frameIndex;
    std::vector<sf::IntRect> frames;

public:
    GameSprite() = delete; // prevent creating without an image
    GameSprite(Game* game, const std::string& filename, const int numFrames = 1, const float x = 0, const float y = 0);
    virtual void enter(); // runs when the game state that this object belongs to becomes active
    virtual void leave(); // runs when the game state that this object belongs to goes inactive
    virtual void draw(); // sprite-level implementation provided; can be overridden by derived classes
    virtual void update(float timeDelta); // can be overridden by derived classes

    float getWidth() const;
    float getHeight() const;
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
};
