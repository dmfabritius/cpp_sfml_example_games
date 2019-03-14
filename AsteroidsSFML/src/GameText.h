#pragma once
#include "GameEntity.h"

class GameText : public GameEntity<sf::Text> {

public:
    GameText() = delete; // prevent creating without a text string
    GameText(Game* game,
             const std::string& textString,
             const int fontSize,
             const float x = 0,
             const float y = 0);

    virtual void enter(); // runs when the game state that this object belongs to becomes active
    virtual void leave(); // runs when the game state that this object belongs to goes inactive
    virtual void update(float timeDelta);
    virtual void draw();

    std::string getText() const;
    void setText(const std::string& textString);
    void setFontSize(const int fontSize);
    void setFontColor(const sf::Color& color);
    void setFontStyle(const sf::Text::Style& style);
};
