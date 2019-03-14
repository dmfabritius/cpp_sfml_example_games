#pragma once
#include "GameEntity.h"

class GameSprite : public GameEntity<sf::Sprite> {
protected:
    sf::Texture image;
    int numFrames; // sprite images can be composites of multiple frames in a horizontal row
    int frameIndex;
    std::vector<sf::IntRect> frames;

public:
    GameSprite() = delete; // prevent creating without an image
    GameSprite(Game* game,
               const std::string& filename,
               const int numFrames = 1,
               const float x = 0,
               const float y = 0);

    virtual void enter(); // runs when the game state that this object belongs to becomes active
    virtual void leave(); // runs when the game state that this object belongs to goes inactive
    virtual void update(float timeDelta); // can be overridden by derived classes
    virtual void draw(); // sprite-level implementation provided; can be overridden by derived classes

    virtual float getWidth() const;

};
