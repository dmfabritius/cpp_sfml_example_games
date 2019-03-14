#pragma once
#include "pch.h"
#include "SFMLAudio.h"
#include "ObjectManager.h"

// forward declarations
class Ship;
class LaserPulse;
class Asteroid;
class GameText;

class Game {
private:
    static const int NUM_STATES = 4; // don't forget to change the number of states if you add/remove a GameState
    const enum GameStates { Splash, Play, Over, Exit };
    GameStates state;
    ObjectManager objectManager[NUM_STATES]; // a set of SFML objects for each state
    tgui::Gui guiManager[NUM_STATES]; // a set of TGUI widgets for each state
    tgui::Theme theme;
    sf::RenderWindow window;
    sf::Event event;
    sf::Font font;
    SFMLAudio audio;
    Ship* ship;
    GameText* scoreDisplay;
    std::vector<LaserPulse*> laserPulses;
    int numAsteroids = 0;
    int score = 0;
    int gamePlaySpeed;

public:
    static const int SCREEN_WIDTH = 1024;
    static const int SCREEN_HEIGHT = 768;

    Game();
    ~Game();
    void run();

    sf::RenderWindow& getWindow();
    tgui::Gui& getOverGui();
    SFMLAudio& getAudio();
    const sf::Font& getFont() const;
    template <typename T> T* getPlayObject(const std::string& name) {
        return dynamic_cast<T*>(objectManager[GameStates::Play].find(name));
    }
    template <typename T> T* getOverObject(const std::string& name) {
        return dynamic_cast<T*>(objectManager[GameStates::Over].find(name));
    }
    void addPlayObject(GameObject* gameObject, std::string name = "");
    void setStateGameOver();

    void createAsteroids(int count = 1);
    void addAsteroid(const float radius, const sf::Vector2f& position);
    void removeAsteroid();
    int getNumAsteroids() const;
    std::vector<LaserPulse*>& getLaserPulses();
    void addLaserPulse(const sf::Vector2f& position, float heading);
    void removeLaserPulse(LaserPulse* pulse);
    unsigned int getNumLaserPulses();
    int getScore() const;
    void updateScore(int points);
    void resetPlay();

private:
    void loadFonts();
    void initializeSound();
    void changeState(const GameStates& state);

    void createSplashObjects();
    void createPlayObjects();
    void createGameOverObjects();

    void handleEvent();
    void handleSplashEvent();
    void handlePlayEvent();
    void handleGameOverEvent();
    void handleCommonEvent();

public:
    static int rand(int min, int max);
    static float randf(int min, int max);
    static sf::Vector2f rand2f();
    static sf::Vector2f angleToVector2f(float angle);
    static float magnitude2f(const sf::Vector2f& vector);
    static float distance2f(const sf::Vector2f& v1, const sf::Vector2f& v2);
    static float distance2f(const sf::Vector2f& v1, float x, float y);
    static float distance2f(float x1, float y1, float x2, float y2);
};
