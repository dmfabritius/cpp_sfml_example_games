#include "pch.h"
#include "GetReady.h"

GetReady::GetReady(Game* game, const std::string& textString, const int fontSize, const float x, const float y) :
    GameText(game, textString, fontSize, x, y) {
}

void GetReady::enter() {
    elapsedTime = 0;
    active = true;
}

void GetReady::update(float timeDelta) {
    timeDelta = std::min(timeDelta, 0.02f);  // prevent huge time gaps
    elapsedTime += timeDelta;
    if (elapsedTime > 2.0f) {
        active = false;
    } else if (elapsedTime > 1.75f) {
        setText("Go!!!");
    } else if (elapsedTime > 1.0f) {
        setText("Set...");
    } else {
        setText("Ready...");
    }
    setPosition(Game::SCREEN_WIDTH * 0.5f - getWidth() * 0.5f, 250.0f);
}
