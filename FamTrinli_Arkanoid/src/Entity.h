#pragma once

class Entity {
public:
    EntityType type;
    sf::Sprite sprite; // the texture, sprite, and animation info
    float x, y; // the position
    float w, h; // width and height of sprite
    float dx = 0, dy = 0; // the change in position

    Entity(EntityType type, sf::Sprite& sprite, float x = 0, float y = 0) :
        type(type), sprite(sprite), x(x), y(y) {
        w = sprite.getGlobalBounds().width;
        h = sprite.getGlobalBounds().height;
        this->sprite.setPosition(x, y);
    }
    virtual void update(){}; // each child class can implement its own update function
    void draw(sf::RenderWindow& window) {
        sprite.setPosition(x, y);
        window.draw(sprite);
    }
};