#include "pch.h"
#include "Game.h"
#include "ServiceManager.h"
#include "GetReady.h"
#include "PlayerPaddle.h"
#include "AIPaddle.h"
#include "GameBall.h"
#include "Brick.h"
#include "GameOver.h"

Game::Game() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Breakout! SFML v2.5.1 TGUI v0.8");
    window.setFramerateLimit(60); // time between each frame should be right around 16.67ms
    window.setVerticalSyncEnabled(true); // should hopefully result in smoother rendering
    initializeSound();
    ServiceManager::Audio()->playSong("audio/soundtrack.ogg", true); // background music
    loadFonts();

    sf::Vector2f a(10, 10), b(20, 20);
    float dist = std::hypot(a.x - b.x, a.y - b.y);

    for (auto& gui : guiManager) {
        gui.setTarget(window); // attach each set of widgets to the game window
        gui.setFont(font);
    }
    theme.load("theme/black.gui"); // load visual theme used by the widgets

    createSplashObjects();
    createPlayObjects();
    createGameOverObjects();

    state = Game::Splash; // setDemoMode out on the splash screen
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
    ServiceManager::Audio()->loadSound("audio/blip.wav");
    ServiceManager::Audio()->loadSound("audio/kaboom.wav");
    ServiceManager::Audio()->loadSong("audio/soundtrack.ogg");
}
void Game::setDemoMode(bool mode) {
    demoMode = mode;
    resetPlay();
}
void Game::resetPlay() {
    score = 0;
    updateScore(0);
    objectManager[GameStates::Play].clear();
    createPlayObjects();
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
GameBall* Game::getBall() {
    return ball;
}
PlayerPaddle* Game::getPaddle() {
    return paddle;
}
int Game::getScore() const {
    return score;
}void Game::updateScore(int points) {
    score += points;
    std::stringstream text;
    text << "Score : " << score;
    scoreText->setText(text.str());
}
bool Game::isAllBricksDestroyed() {
    return objectManager[GameStates::Play].size() < 5;
}
void Game::createBricks() {
    Brick* brick;
    for (int col = 0; col < 10; col++) {
        std::stringstream name;
        name << "R" << col;
        brick = new Brick(this, 35.0f + 100.0f * col, 100.0f, sf::Color::Red, 40);
        objectManager[GameStates::Play].add(name.str(), brick);
    }
    for (int col = 0; col < 10; col++) {
        std::stringstream name;
        name << "O" << col;
        brick = new Brick(this, 85.0f + 100.0f * col, 130.0f, sf::Color(255, 153, 0), 30);
        objectManager[GameStates::Play].add(name.str(), brick);
    }
    for (int col = 0; col < 10; col++) {
        std::stringstream name;
        name << "Y" << col;
        brick = new Brick(this, 35.0f + 100.0f * col, 160.0f, sf::Color::Yellow, 20);
        objectManager[GameStates::Play].add(name.str(), brick);
    }
    for (int col = 0; col < 10; col++) {
        std::stringstream name;
        name << "G" << col;
        brick = new Brick(this, 85.0f + 100.0f * col, 190.0f, sf::Color::Green, 10);
        objectManager[GameStates::Play].add(name.str(), brick);
    }
}
void Game::setStateGameOver() {
    changeState(GameStates::Over);
}
void Game::changeState(const GameStates & state) {
    objectManager[this->state].leave(); // clean-up/reset all objects for the current game state
    this->state = state;
    objectManager[state].enter(); // set-up/initialize all objects for the new game state
}

void Game::createSplashObjects() {
    GameSprite* splash = new GameSprite(this, "images/splashscreen.jpg", 1,
        Game::SCREEN_WIDTH  * 0.5f, Game::SCREEN_HEIGHT  * 0.5f);
    objectManager[GameStates::Splash].add("Splash", splash);

    tgui::Button::Ptr demo = tgui::Button::create("Demo");
    guiManager[GameStates::Splash].add(demo, "demo");
    demo->setRenderer(theme.getRenderer("Button"));
    demo->setSize({ 200, 70 });
    demo->setPosition({ 120, 540 });
    demo->setTextSize(40);
    demo->connect("pressed", &Game::setDemoMode, this, true);

    tgui::Button::Ptr play = tgui::Button::create("Play");
    guiManager[GameStates::Splash].add(play, "play");
    play->setRenderer(theme.getRenderer("Button"));
    play->setSize({ 200, 70 });
    play->setPosition({ 400, 540 });
    play->setTextSize(40);
    play->connect("pressed", &Game::setDemoMode, this, false);

    tgui::Button::Ptr quit = tgui::Button::create("Quit");
    guiManager[GameStates::Splash].add(quit, "quit");
    quit->setRenderer(theme.getRenderer("Button"));
    quit->setSize({ 200, 70 });
    quit->setPosition({ 680, 540 });
    quit->setTextSize(40);
    quit->connect("pressed", &Game::changeState, this, GameStates::Exit);
}
void Game::createPlayObjects() {
    scoreText = new GameText(this, "Score: 0", 24, 20, 725);
    objectManager[GameStates::Play].add("Score", scoreText);

    GetReady* getReady = new GetReady(this, "Ready...", 48, 0, 0);
    objectManager[GameStates::Play].add("GetReady", getReady);

    ball = new GameBall(this);
    objectManager[GameStates::Play].add("Ball", ball);

    paddle = (demoMode)
        ? paddle = new AIPaddle(this, Game::SCREEN_WIDTH / 2, 700)
        : paddle = new PlayerPaddle(this, Game::SCREEN_WIDTH / 2, 700);
    objectManager[GameStates::Play].add("Paddle", paddle);

    createBricks();
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
    GameOver* gameover = new GameOver(this, "", 32, 280, 220);
    gameover->setFontColor(sf::Color(4, 147, 251));
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
        if (event.type == sf::Event::EventType::MouseButtonReleased) changeState(GameStates::Play);
}
void Game::handlePlayEvent() {
    // Note: each game object may also handle object-specific events in its update() method
    //       e.g., PlayerPaddle::update() handles left & right key presses
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
        changeState(GameStates::Splash);
}
void Game::handleGameOverEvent() {
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Escape)
            changeState(GameStates::Exit);
        else if (event.key.code == sf::Keyboard::Space)
            changeState(GameStates::Play);
    }
    if (event.type == sf::Event::EventType::MouseButtonReleased)
        changeState(GameStates::Play);
}
void Game::handleCommonEvent() {
    if (event.type == sf::Event::Closed) // if the user closes the window, exit the game
        changeState(GameStates::Exit);
}
