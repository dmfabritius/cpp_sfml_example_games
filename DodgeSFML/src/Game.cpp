#include "pch.h"
#include "Game.h"
#include "ServiceManager.h"
#include "GameSprite.h"
#include "GameText.h"
#include "GetReady.h"
#include "GameOver.h"
#include "Ship.h"
#include "Asteroid.h"

Game::Game() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pang! SFML v2.5.1 TGUI v0.8");
    window.setFramerateLimit(60); // time between each frame should be right around 16.67ms
    window.setVerticalSyncEnabled(true); // should hopefully result in smoother rendering
    initializeSound();
    ServiceManager::Audio()->playSong("audio/solveThePuzzle.ogg", true); // background music
    loadFonts();

    for (auto& gui : guiManager) {
        gui.setTarget(window); // attach each set of widgets to the game window
        gui.setFont(font);
    }
    theme.load("theme/black.gui"); // load visual theme used by the widgets

    createSplashObjects();
    createPlayObjects();
    createGameOverObjects();

    state = Game::Splash; // start out on the splash screen
    //state = Game::Play; // skip splash screen and menu, go straight to game play
}
Game::~Game() {
    ServiceManager::Audio()->stopAll();
    window.close();
}

void Game::loadFonts() {
    // just one font for now, but maybe we'll want more later
    if (!font.loadFromFile("fonts/ocr.ttf")) {
        throw std::runtime_error("OCR font was not found in Game::loadFonts");
    }
}
void Game::initializeSound() {
    ServiceManager::registerAudioProvider(&soundProvider);

    // pre-load sounds during initialization so there is no delay during game play
    ServiceManager::Audio()->loadSound("audio/crash.wav");
    ServiceManager::Audio()->loadSound("audio/thrust.wav");
    ServiceManager::Audio()->loadSong("audio/solveThePuzzle.ogg");
}

void Game::createSplashObjects() {
    GameSprite* splash = new GameSprite(this, "images/splashscreen.jpg", 1,
        Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
    objectManager[GameStates::Splash].add("Splash", splash);
}
void Game::createPlayObjects() {
    GetReady* getReady = new GetReady(this, "Ready...", 48, 0, 0);
    objectManager[GameStates::Play].add("GetReady", getReady);

    scoreText = new GameText(this, "Score: 0", 24, 20, 725);
    objectManager[GameStates::Play].add("Score", scoreText);

    objectManager[GameStates::Play].add("Ship", new Ship(this));

    Asteroid* asteroid = new Asteroid(this, "images/a1.png");
    objectManager[GameStates::Play].add("A1", asteroid);
    asteroid = new Asteroid(this, "images/a2.png");
    objectManager[GameStates::Play].add("A2", asteroid);
    asteroid = new Asteroid(this, "images/a3.png");
    objectManager[GameStates::Play].add("A3", asteroid);
    asteroid = new Asteroid(this, "images/a4.png");
    objectManager[GameStates::Play].add("A4", asteroid);
    asteroid = new Asteroid(this, "images/a1.png");
    objectManager[GameStates::Play].add("A5", asteroid);
    asteroid = new Asteroid(this, "images/a2.png");
    objectManager[GameStates::Play].add("A6", asteroid);
    asteroid = new Asteroid(this, "images/a3.png");
    objectManager[GameStates::Play].add("A7", asteroid);
    asteroid = new Asteroid(this, "images/a4.png");
    objectManager[GameStates::Play].add("A8", asteroid);
}
void Game::createGameOverObjects() {
    auto name = tgui::EditBox::create();
    guiManager[GameStates::Over].add(name, "playername");
    name->setRenderer(theme.getRenderer("EditBox"));
    name->setSize({ 200, 48 });
    name->setPosition({ 325, 610 });
    name->setDefaultText("Player name");
    name->setTextSize(0); // auto scale the font to match the size of the EditBox

    auto submit = tgui::Button::create("Save");
    guiManager[GameStates::Over].add(submit, "submit");
    submit->setRenderer(theme.getRenderer("Button"));
    submit->setSize({ 100, 48 });
    submit->setPosition({ 575, 610 });
    submit->setTextSize(28);
    submit->connect("pressed", GameOver::saveScore, this);

    // the gameover object constructor references the playername & submit objects,
    // so it must be created after they are
    GameOver* gameover = new GameOver(this, " ", 40, 225, 60);
    gameover->setFontColor(sf::Color::Magenta);
    gameover->setFontStyle(sf::Text::Bold);
    objectManager[GameStates::Over].add("GameOver", gameover);

    std::cout << "Initiating communications with high score server...";
    gameover->loadHighScores(); // request scores from database server, which goes to sleep when it hasn't been used for a while
    std::cout << "completed.\n";
}

void Game::handleEvent() {
    guiManager[state].handleEvent(event); // allow widgets to react to event
    switch (state) {
        case GameStates::Splash: handleSplashEvent(); break;
        case GameStates::Play: handlePlayEvent(); break;
        case GameStates::Over: handleGameOverEvent(); break;
    }
    handleCommonEvent();
}
void Game::handleSplashEvent() {
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Escape)
            changeState(GameStates::Exit);
        else
            changeState(GameStates::Play);
    } else
        if (event.type == sf::Event::EventType::MouseButtonReleased)
            changeState(GameStates::Play);
}
void Game::handlePlayEvent() {
    // Note: each game object may also handle object-specific events in its update() method
    //       e.g., PlayerPaddle::update() handles left & right key presses
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
        changeState(GameStates::Splash);
}
void Game::handleGameOverEvent() {
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Escape) changeState(GameStates::Exit);
        if (event.key.code == sf::Keyboard::Space ||
            event.key.code == sf::Keyboard::Enter) changeState(GameStates::Play);
    } else
        if (event.type == sf::Event::EventType::MouseButtonReleased) changeState(GameStates::Play);
}
void Game::handleCommonEvent() {
    if (event.type == sf::Event::Closed) // if the user closes the window, exit the game
        changeState(GameStates::Exit);
}

void Game::run() {
    while (state != GameStates::Exit) {
        while (window.pollEvent(event)) handleEvent();

        objectManager[state].update();

        window.clear();
        if (state == GameStates::Over) objectManager[GameStates::Play].draw(); // display final game scene
        objectManager[state].draw(); // draw all SFML objects for the current game state
        guiManager[state].draw(); // draw all TGUI widgets for the current game state
        window.display();
    }
}
sf::RenderWindow& Game::getWindow() {
    return window;
}
tgui::Gui& Game::getOverGui() {
    return guiManager[GameStates::Over];
}
const sf::Font& Game::getFont() {
    return font;
}
int Game::getScore() const {
    return score;
}
void Game::setStateGameOver() {
    changeState(GameStates::Over);
}
void Game::updateScore(int points) {
    score += points;
    std::stringstream text;
    text << "Score : " << score;
    scoreText->setText(text.str());
}
void Game::resetPlay() {
    score = 0;
    updateScore(0);
    objectManager[GameStates::Play].clear();
    createPlayObjects();
}

void Game::changeState(const GameStates & state) {
    objectManager[this->state].leave(); // clean-up/reset all objects for the current game state
    this->state = state;
    objectManager[state].enter(); // set-up/initialize all objects for the new game state
}

int Game::rand(int min, int max) {
    return min + std::rand() % (1 + max - min);
}
float Game::randf(int min, int max) {
    return static_cast<float>(rand(min, max));
}
sf::Vector2f Game::rand2f() { // random 2D unit vector
    return sf::Vector2f(
        Game::rand(-1000, 1000) * 0.001f,
        Game::rand(-1000, 1000) * 0.001f);
}
sf::Vector2f Game::angleToVector2f(float angle) {
    sf::Vector2f vector;
    float theta = (angle - 90.0f) / 180.0f * PI; // all our images are oriented pointing up, so adjust the angle by 90 degrees
    vector.x = std::cos(theta);
    vector.y = std::sin(theta);
    return vector;
}
float Game::magnitude2f(const sf::Vector2f& vector) {
    return std::hypotf(vector.x, vector.y);
}
float Game::distance2f(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    return distance2f(v1.x, v1.y, v2.x, v2.y);
}
float Game::distance2f(const sf::Vector2f& v1, float x, float y) {
    return distance2f(v1.x, v1.y, x, y);
}
float Game::distance2f(float x1, float y1, float x2, float y2) {
    return std::hypotf(x1 - x2, y1 - y2);
}
