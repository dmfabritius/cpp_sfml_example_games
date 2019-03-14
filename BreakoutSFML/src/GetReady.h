#pragma once
#include "Game.h"
#include "GameText.h"

class GetReady : public GameText {
private:
    float elapsedTime;

public:
    GetReady() = delete; // prevent creating without a text string
    GetReady(Game* game, const std::string& textString, const int fontSize, const float x, const float y);

    void enter(); // runs when the game state that this object belongs to becomes active
    void update(float timeDelta);
};
