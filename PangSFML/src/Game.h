#pragma once
#include "pch.h"
#include "SFMLSoundProvider.h"
#include "ObjectManager.h"
#include "MainMenu.h"

class Game {
private:
    static const int NUM_STATES = 5; // don't forget to change the number of states if you add/remove a GameState
    const enum GameStates { Splash, Menu, Play, Over, Exit };
    GameStates state;
    ObjectManager objectManager[NUM_STATES]; // a set of SFML objects for each state
    tgui::Gui guiManager[NUM_STATES]; // a set of TGUI widgets for each state
    tgui::Theme theme;
    sf::RenderWindow window;
    sf::Event event;
    sf::Font font;
    MainMenu menu;
    SFMLSoundProvider soundProvider;

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
    void setStateGameOver();

private:
    void loadFonts();
    void initializeSound();
    void changeState(const GameStates& state);

    void createSplashObjects();
    void createMenuObjects();
    void createPlayObjects();
    void createGameOverObjects();

    void handleEvent();
    void handleSplashEvent();
    void handleMenuEvent();
    void handlePlayEvent();
    void handleGameOverEvent();
    void handleCommonEvent();
};
