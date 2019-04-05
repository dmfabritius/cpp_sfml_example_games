#pragma once

class Block : public Entity {
public:
    Block(sf::Sprite& sprite, float x, float y) :
        Entity(BLOCK, sprite, x, y) {
    }
    // blocks don't move or animate or anything
};