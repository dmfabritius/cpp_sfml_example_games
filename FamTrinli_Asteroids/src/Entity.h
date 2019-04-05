#pragma once

class Entity {
public:
    enum EntityType { SHIP, LASER, ASTEROID, EXPLOSION };
    EntityType type;
    AnimatedSprite anim; // the texture, sprite, and animation info
    float x, y, angle; // the position and heading (in degrees, because that's a little easier to visualize)
    float dx = 0, dy = 0; // the change in position
    bool active = true; // when false, will be deleted by Game::updateEntities()

    Entity(EntityType type, AnimatedSprite& anim, float x = 0, float y = 0, float angle = 0) :
        type(type), anim(anim), x(x), y(y), angle(angle) {
    }
    virtual void update() = 0; // each child class implements its own update function
    void draw(sf::RenderWindow& window) {
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90); // set the angle and correct for all sprites being designed pointing upwards
        window.draw(anim.sprite);
    }
};