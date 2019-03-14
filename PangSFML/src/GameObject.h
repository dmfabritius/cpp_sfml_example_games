#pragma once
#include "pch.h"

class Game; // forward declaration

class GameObject {
protected:
    Game* game;
    bool active;

public:
    virtual void enter() = 0; // runs when the game state that this object belongs to becomes active
    virtual void leave() = 0; // runs when the game state that this object belongs to goes inactive
    virtual void draw() = 0;
    virtual void update(float timeDelta) = 0;

    GameObject(Game* game);
    bool isActive();
};
