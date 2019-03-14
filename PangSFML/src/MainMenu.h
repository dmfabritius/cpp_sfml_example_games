#pragma once
#include "pch.h"

class MainMenu {
public:
    const enum MenuChoices { Nothing, Play, Exit };
private:
    struct MenuItem {
        int left, right, top, bottom;
        MenuChoices response;

        MenuItem(int left, int right, int top, int bottom, MenuChoices response);
    };
    std::vector<MenuItem> menuItems;

public:
    MainMenu();
    MenuChoices getChoice(int x, int y);
};