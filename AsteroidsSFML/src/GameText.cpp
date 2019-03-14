#include "pch.h"
#include "GameText.h"

GameText::GameText(Game* game, const std::string& textString, const int fontSize, const float x, const float y) :
    GameEntity(game) {
    entity.setString(textString);
    entity.setCharacterSize(fontSize);
    entity.setPosition(x, y);
    entity.setFont(game->getFont());
}

void GameText::enter() {
    // generic text doesn't have any set-up/initialization actions
    // objects derived from this class can implement their own
}

void GameText::leave() {
    // generic text doesn't have any clean-up actions
    // objects derived from this class can implement their own
}

void GameText::update(float timeDelta) {
    // generic text doesn't have any changes/animations to update
    // objects derived from this class can implement their own
}
void GameText::draw() {
    if (active) game->getWindow().draw(entity);
}
std::string GameText::getText() const {
    return entity.getString();
}

void GameText::setText(const std::string& textString) {
    entity.setString(textString);
}

void GameText::setFontSize(const int fontSize) {
    entity.setCharacterSize(fontSize);
}

void GameText::setFontColor(const sf::Color& color) {
    entity.setFillColor(color);
}

void GameText::setFontStyle(const sf::Text::Style& style) {
    entity.setStyle(style);
}
