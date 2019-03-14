#include "pch.h"
#include "Game.h"
#include "SFMLAudio.h"
#include "GetReady.h"
#include "GameOver.h"
#include "Ship.h"
#include "LaserPulse.h"
#include "Asteroid.h"

Game::Game() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Ast*roids");
    gamePlaySpeed = 60;
    window.setFramerateLimit(gamePlaySpeed);
    initializeSound();
    loadFonts();

    for (auto& gui : guiManager) {
        gui.setTarget(window); // attach the game window to each set of gui widgets
        gui.setFont(font);
    }
    theme.load("theme/black.gui"); // load visual theme that will be used by the widgets

    createSplashObjects();
    createPlayObjects();
    createGameOverObjects();

    state = Game::Splash; // start out on the splash screen
    //state = Game::Play; // go straight to game play
}
Game::~Game() {
    audio.stopAll();
    window.close();
}

void Game::initializeSound() {
    // pre-load sounds during initialization so there is no delay during game play
    audio.loadSound("audio/bangLarge.wav");
    audio.loadSound("audio/bangMedium.wav");
    audio.loadSound("audio/bangSmall.wav");
    audio.loadSound("audio/fire.wav");
    audio.loadSound("audio/thrust.wav");

    // https://patrickdearteaga.com/arcade-music/
    audio.playSong("audio/solveThePuzzle.ogg", true); // load and start looping background music
}
void Game::loadFonts() {
    // just one font for now, but maybe we'll want more later
    if (!font.loadFromFile("fonts/ocr.ttf")) {
        throw std::runtime_error("unable to load OCR font in Game::loadFonts");
    }
}

void Game::createSplashObjects() {
    // https://cdn1.img.sputniknews.com/images/102180/25/1021802507.jpg
    GameSprite* splash = new GameSprite(this, "images/splashscreen.jpg");
    objectManager[GameStates::Splash].add(splash, "Splash");

    tgui::Button::Ptr play = tgui::Button::create("Play");
    guiManager[GameStates::Splash].add(play, "play");
    play->setRenderer(theme.getRenderer("Button"));
    play->setSize({ 250, 80 });
    play->setPosition({ 700, 350 });
    play->setTextSize(44);
    play->connect("pressed", &Game::changeState, this, GameStates::Play);

    tgui::Button::Ptr quit = tgui::Button::create("Quit");
    guiManager[GameStates::Splash].add(quit, "quit");
    quit->setRenderer(theme.getRenderer("Button"));
    quit->setSize({ 250, 80 });
    quit->setPosition({ 700, 450 });
    quit->setTextSize(44);
    quit->connect("pressed", &Game::changeState, this, GameStates::Exit);
}
void Game::createPlayObjects() {
    GetReady* getReady = new GetReady(this, "Ready...", 48);
    objectManager[GameStates::Play].add(getReady, "GetReady");

    scoreDisplay = new GameText(this, "Score : 0", 28, 20, 20);
    objectManager[GameStates::Play].add(scoreDisplay, "Score");

    ship = new Ship(this);
    objectManager[GameStates::Play].add(ship, "Ship");
    createAsteroids(4);
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
    GameOver* gameover = new GameOver(this, "", 40, 225, 60);
    gameover->setFontColor(sf::Color::Magenta);
    gameover->setFontStyle(sf::Text::Bold);
    objectManager[GameStates::Over].add(gameover, "GameOver");

    // the highscore database server goes to sleep when it hasn't been used for a while
    // so on a separate, non-blocking thread, send it a request to wake it up
    std::thread(&GameOver::loadHighScores, gameover).detach();
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
        if (event.key.code == sf::Keyboard::Space) changeState(GameStates::Play);
        if (event.key.code == sf::Keyboard::Escape) changeState(GameStates::Exit);
    }
    if (event.type == sf::Event::EventType::MouseButtonReleased) {
        changeState(GameStates::Play);
    }
}
void Game::handlePlayEvent() {
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Escape) changeState(GameStates::Splash);
        if (event.key.code == sf::Keyboard::Space) ship->fireLaserPulse();
    }
}
void Game::handleGameOverEvent() {
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
        changeState(GameStates::Splash);
    if (event.type == sf::Event::EventType::MouseButtonReleased)
        changeState(GameStates::Play);
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
SFMLAudio& Game::getAudio() {
    return audio;
}
const sf::Font& Game::getFont() const {
    return font;
}
void Game::addPlayObject(GameObject* gameObject, std::string name) {
    objectManager[GameStates::Play].add(gameObject, name);
}
void Game::changeState(const GameStates& state) {
    objectManager[this->state].leave(); // clean-up/reset all objects for the current game state
    this->state = state;
    objectManager[state].enter(); // set-up/initialize all objects for the new game state
}
void Game::setStateGameOver() {
    changeState(GameStates::Over);
}

void Game::createAsteroids(int count) {
    numAsteroids += count;
    for (int i = 0; i < count; i++) {
        objectManager[GameStates::Play].add(new Asteroid(this));
    }

    gamePlaySpeed += 5; // speed up the game when asteroids are added
    window.setFramerateLimit(gamePlaySpeed);
}
void Game::addAsteroid(const float radius, const sf::Vector2f& position) {
    objectManager[GameStates::Play].add(new Asteroid(this, radius, position));
    numAsteroids++;
}
void Game::removeAsteroid() {
    numAsteroids--;
}
int Game::getNumAsteroids() const {
    return numAsteroids;
}
std::vector<LaserPulse*>& Game::getLaserPulses() {
    return laserPulses;
}
void Game::addLaserPulse(const sf::Vector2f& position, float heading) {
    LaserPulse* pulse = new LaserPulse(this, position, heading);
    laserPulses.push_back(pulse);
    objectManager[GameStates::Play].add(pulse);
}
void Game::removeLaserPulse(LaserPulse* pulse) {
    auto it = std::find(laserPulses.begin(), laserPulses.end(), pulse);
    //if (it == laserPulses.end()) throw std::runtime_error("pulse not found");
    laserPulses.erase(it);
}
unsigned int Game::getNumLaserPulses() {
    return laserPulses.size();
}
int Game::getScore() const {
    return score;
}
void Game::updateScore(int points) {
    score += points;
    std::stringstream text;
    text << "Score : " << score;
    scoreDisplay->setText(text.str());
}
void Game::resetPlay() {
    gamePlaySpeed = 60;
    window.setFramerateLimit(gamePlaySpeed); // time between each frame should be right around 16.67ms
    numAsteroids = 0;
    score = 0;
    updateScore(0);
    laserPulses.clear();
    objectManager[GameStates::Play].clear();
    createPlayObjects();
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
