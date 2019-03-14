#pragma once
#include "pch.h"
#include "SFMLSoundProvider.h"
#include "ObjectManager.h"

// forward declarations
class GameText;
class GameBall;
class PlayerPaddle;

class Game {
private:
    static const int NUM_STATES = 5; // don't forget to change the number of states if you add/remove a GameState
    const enum GameStates { Splash, Play, Over, Exit };
    GameStates state;
    ObjectManager objectManager[NUM_STATES]; // a set of SFML objects for each state
    tgui::Gui guiManager[NUM_STATES]; // a set of TGUI widgets for each state
    tgui::Theme theme;
    sf::RenderWindow window;
    sf::Event event;
    sf::Font font;
    SFMLSoundProvider soundProvider;
    int score;
    GameText* scoreText;
    GameBall* ball;
    PlayerPaddle* paddle;
    bool demoMode;

public:
    static const int SCREEN_WIDTH = 1024;
    static const int SCREEN_HEIGHT = 768;

    Game();
    ~Game();
    void run();

    sf::RenderWindow& getWindow();
    tgui::Gui& getOverGui();
    template <typename T> T* getPlayObject(const std::string& name) {
        return dynamic_cast<T*>(objectManager[GameStates::Play].find(name));
    }
    template <typename T> T* getOverObject(const std::string& name) {
        return dynamic_cast<T*>(objectManager[GameStates::Over].find(name));
    }
    const sf::Font& getFont();
    GameBall* getBall();
    PlayerPaddle* getPaddle();
    int getScore() const;
    void updateScore(int points);
    void setStateGameOver();
    bool isAllBricksDestroyed();
    void createBricks();
    void resetPlay();

private:
    void loadFonts();
    void initializeSound();
    void setDemoMode(bool mode);
    void changeState(const GameStates& state);

    void createSplashObjects();
    void createPlayObjects();
    void createGameOverObjects();

    void handleEvent();
    void handleSplashEvent();
    void handlePlayEvent();
    void handleGameOverEvent();
    void handleCommonEvent();
};
