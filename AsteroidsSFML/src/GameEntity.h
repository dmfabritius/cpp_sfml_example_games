#pragma once
#include "Game.h"
#include "GameObject.h"

template <class T> // can be sf::CircleShape, sf::ConvexShape, sf::RectangleShape, sf::Sprite, sf::Text
class GameEntity : public GameObject {
protected:
    T entity;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

public:
    GameEntity() = delete;
    GameEntity(Game* game,
               const sf::Vector2f& velocity = sf::Vector2f(0, 0),
               const sf::Vector2f& acceleration = sf::Vector2f(0, 0)) :
        GameObject(game), velocity(velocity), acceleration(acceleration) {
    }

    virtual void enter() {} // runs when the game state that this object belongs to becomes active
    virtual void leave() {}; // runs when the game state that this object belongs to goes inactive
    virtual void update(float timeDelta) {} // most often this will be implemented by derived class
    virtual void draw() { if (active) game->getWindow().draw(entity); }

    virtual float getWidth() const { return entity.getGlobalBounds().width; } // overwritten by GameSprite
    float getHeight() const { return entity.getGlobalBounds().height; }
    float getLeft() const { return entity.getGlobalBounds().left; }
    float getRight() const { return entity.getGlobalBounds().left + getWidth(); }
    float getTop() const { return entity.getGlobalBounds().top; }
    float getBottom() const { return entity.getGlobalBounds().top + getHeight(); }
    sf::FloatRect getGlobalBounds() const { return entity.getGlobalBounds(); }
    sf::FloatRect getLocalBounds() const { return entity.getLocalBounds(); }
    sf::Vector2f getPosition() const { return entity.getPosition(); }
    void setPosition(const sf::Vector2f& position) { entity.setPosition(position); }
    void setPosition(float x, float y) { entity.setPosition(x, y); }

    // to work correctly, the origin of the entity must be its center
    void wrap() {
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
};
