#pragma once
#include "pch.h"
#include "GameObject.h"

class GameText : public GameObject {
private:
    sf::Text text;

public:
    GameText() = delete; // prevent creating without a text string
    GameText(Game* game, const std::string& textString, const int fontSize, const float x, const float y);
    virtual void enter(); // runs when the game state that this object belongs to becomes active
    virtual void leave(); // runs when the game state that this object belongs to goes inactive
    virtual void update(float timeDelta);
    virtual void draw();

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

    std::string getText() const;
    void setText(const std::string& textString);
    void setFontSize(const int fontSize);
    void setFontColor(const sf::Color& color);
    void setFontStyle(const sf::Text::Style& style);
};
