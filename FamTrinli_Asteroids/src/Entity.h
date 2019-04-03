#pragma once

class Entity {
public:
    std::string type; // the type of entity: ship, laser, asteroid, explosion
    AnimatedSprite anim; // the texture, sprite, and animation info
    float x, y, angle; // the position and heading (in degrees, because that's a little easier to visualize)
    float dx = 0, dy = 0; // the change in position
    bool active = true; // flag to mark entity for deletion

    Entity(std::string type, AnimatedSprite& anim, float x = 0, float y = 0, float angle = 0) :
        type(type), anim(anim), x(x), y(y), angle(angle) {
    }
    virtual void update() {
        // each child class implements its own update function
    };
    void draw(sf::RenderWindow& window) {
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90); // set the angle and adjust for all sprites being designed pointing upwards
        window.draw(anim.sprite);
    }
};