#include "pch.h"
#include "Game.h"
#include "ServiceManager.h"
#include "GetReady.h"
#include "PlayerPaddle.h"
#include "AIPaddle.h"
#include "GameBall.h"
#include "GameOver.h"

Game::Game() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pang! SFML v2.5.1 TGUI v0.8");
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
    createMenuObjects();
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
    ServiceManager::Audio()->loadSound("audio/blip.wav");
    ServiceManager::Audio()->loadSound("audio/kaboom.wav");
    ServiceManager::Audio()->loadSong("audio/soundtrack.ogg");
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
void Game::setStateGameOver() {
    changeState(GameStates::Over);
}

void Game::changeState(const GameStates & state) {
    objectManager[this->state].leave(); // clean-up/reset all objects for the current game state
    this->state = state;
    objectManager[state].enter(); // set-up/initialize all objects for the new game state
}

void Game::createSplashObjects() {
    GameSprite* splash = new GameSprite(this, "images/splashscreen.png");
    objectManager[GameStates::Splash].add("Splash", splash);
}
void Game::createMenuObjects() {
    GameSprite* menu = new GameSprite(this, "images/menuscreen.png");
    objectManager[GameStates::Menu].add("MainMenu", menu);
}
void Game::createPlayObjects() {
    PlayerPaddle* player1 = new PlayerPaddle(this, Game::SCREEN_WIDTH / 2, 700);
    //AIPaddle* player1 = new AIPaddle(this, Game::SCREEN_WIDTH / 2, 700);
    objectManager[GameStates::Play].add("Paddle1", player1);
    GameText* score1 = new GameText(this, "Player 1 score: 0", 24, 20, 725);
    objectManager[GameStates::Play].add("Score1", score1);

    AIPaddle* player2 = new AIPaddle(this, Game::SCREEN_WIDTH / 2, 40);
    objectManager[GameStates::Play].add("Paddle2", player2);
    GameText* score2 = new GameText(this, "Player 2 score: 0", 24, 740, 725);
    objectManager[GameStates::Play].add("Score2", score2);

    GetReady* getReady = new GetReady(this, "Ready...", 48, 0, 0);
    objectManager[GameStates::Play].add("GetReady", getReady);
    GameBall *ball = new GameBall(this);
    objectManager[GameStates::Play].add("Ball", ball);
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
        case GameStates::Menu: handleMenuEvent(); break;
        case GameStates::Play: handlePlayEvent(); break;
        case GameStates::Over: handleGameOverEvent(); break;
    }
    handleCommonEvent();
}
void Game::handleSplashEvent() {
    if (event.type == sf::Event::EventType::KeyReleased ||
        event.type == sf::Event::EventType::MouseButtonReleased) {
        changeState(GameStates::Menu);
    }
}
void Game::handleMenuEvent() {
    if (event.type == sf::Event::MouseButtonReleased) {
        MainMenu::MenuChoices choice = menu.getChoice(event.mouseButton.x, event.mouseButton.y);
        switch (choice) {
            case MainMenu::Play: changeState(GameStates::Play); break;
            case MainMenu::Exit: changeState(GameStates::Exit); break;
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Space) changeState(GameStates::Play);
        if (event.key.code == sf::Keyboard::Escape) changeState(GameStates::Exit);
    }
}
void Game::handlePlayEvent() {
    // Note: each game object may also handle object-specific events in its update() method
    //       e.g., PlayerPaddle::update() handles left & right key presses
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
        changeState(GameStates::Menu);
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
