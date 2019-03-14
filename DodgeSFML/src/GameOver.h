#pragma once
#include "Game.h"
#include "GameText.h"

class GameOver : public GameText {
private:
    tgui::EditBox::Ptr name;
    tgui::Button::Ptr submit;
    int lowestHighScore;

public:
    GameOver() = delete; // prevent creating without a text string
    GameOver(Game* game, const std::string& textString, const int fontSize, const float x, const float y);

    void enter(); // runs when the game state that this object belongs to becomes active
    void leave(); // runs when the game state that this object belongs to goes inactive
    void loadHighScores();
    void displayHighScores(const std::string& data);

    static void saveScore(Game* game);
};
