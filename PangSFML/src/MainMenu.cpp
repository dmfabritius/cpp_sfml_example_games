#include "pch.h"
#include "Game.h"
#include "MainMenu.h"

MainMenu::MainMenu() {
    // drawing rectangles in the main game/draw loop can help determine the
    // dimensions of the clickable areas
    //sf::RectangleShape rectangle;
    //rectangle.setSize(sf::Vector2f(Game::SCREEN_WIDTH, 560-383));
    //rectangle.setOutlineColor(sf::Color::Red);
    //rectangle.setOutlineThickness(1);
    //rectangle.setPosition(0, 383);
    //Game::window.draw(rectangle);

    //Setup clickable regions
    menuItems.push_back(MenuItem(0, Game::SCREEN_WIDTH, 145, 380, MainMenu::Play));
    menuItems.push_back(MenuItem(0, Game::SCREEN_WIDTH, 383, 560, MainMenu::Exit));
}

MainMenu::MenuChoices MainMenu::getChoice(int x, int y) {
    for (const auto& item : menuItems) {
        if (item.left < x && item.right > x && item.top < y && item.bottom > y) {
            return item.response;
        }
    }
    return Nothing;
}

// construct a clickable area and the response it provides when it's clicked
MainMenu::MenuItem::MenuItem(int left, int right, int top, int bottom, MenuChoices response) :
    left(left),
    right(right),
    top(top),
    bottom(bottom),
    response(response) {
}
