#include "pch.h"
#include "Game.h"
#include "GameText.h"

GameText::GameText(Game* game, const std::string& textString, const int fontSize, const float x, const float y) :
    GameObject(game) {
    text.setString(textString);
    text.setCharacterSize(fontSize);
    text.setPosition(x, y);
    text.setFont(game->getFont());
}

void GameText::enter() {
    // generic game sprites don't have any set-up/initialization actions
    // objects derived from this class can implement their own
}

void GameText::leave() {
    // generic game sprites don't have any clean-up actions
    // objects derived from this class can implement their own
}

void GameText::update(float timeDelta) {
        // not currently animating text objects in any way
}
void GameText::draw() {
    if (active) {
        game->getWindow().draw(text);
    }
}

float GameText::getWidth() const {
    return text.getGlobalBounds().width;
}

float GameText::getHeight() const {
    return text.getGlobalBounds().height;
}

float GameText::getLeft() const {
    return text.getGlobalBounds().left;
}
float GameText::getRight() const {
    return text.getGlobalBounds().left + text.getGlobalBounds().width;
}
float GameText::getTop() const {
    return text.getGlobalBounds().top;
}
float GameText::getBottom() const {
    return text.getGlobalBounds().top + text.getGlobalBounds().height;
}

sf::FloatRect GameText::getGlobalBounds() const {
    return text.getGlobalBounds();
}

sf::Vector2f GameText::getPosition() const {
    return text.getPosition();
}

void GameText::setPosition(const sf::Vector2f & position) {
    text.setPosition(position);
}

void GameText::setPosition(float x, float y) {
    text.setPosition(x, y);
}

std::string GameText::getText() const {
    return text.getString();
}

void GameText::setText(const std::string & textString) {
    text.setString(textString);
}

void GameText::setFontSize(const int fontSize) {
    text.setCharacterSize(fontSize);
}

void GameText::setFontColor(const sf::Color & color) {
    text.setFillColor(color);
}

void GameText::setFontStyle(const sf::Text::Style & style) {
    text.setStyle(style);
}
