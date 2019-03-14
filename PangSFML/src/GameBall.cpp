#include "pch.h"
#include "Game.h"
#include "ServiceManager.h"
#include "GameText.h"
#include "GetReady.h"
#include "GameBall.h"
#include "AIPaddle.h"

GameBall::GameBall(Game* game) :
    GameSprite(game, "images/balls.png", 8),
    paddle1(game->getPlayObject<PlayerPaddle>("Paddle1")),
    paddle2(game->getPlayObject<PlayerPaddle>("Paddle2")),
    getReady(game->getPlayObject<GetReady>("GetReady")) {
    float w = (getWidth() / 2) / numFrames; // image width is 256, width of one frame is 32
    float h = getHeight() / 2;
    sprite.setOrigin(w, h);
    startingPosition.x = Game::SCREEN_WIDTH / 2 - w;
    startingPosition.y = Game::SCREEN_HEIGHT / 2 - h;
}

void GameBall::enter() {
    setPosition(startingPosition);
    velocity = 250.0f;
    angle = (float)(rand() % 90 + 135);
    updateScore(1, paddle1, paddle1->resetScore());
    updateScore(2, paddle2, paddle2->resetScore());
}

void GameBall::update(float timeDelta) {
    if (getReady->isActive()) return;   // wait for "get ready" message to complete before moving the ball
    timeDelta = std::min(timeDelta, 0.05f);  // prevent huge time gaps
    float moveAmount = velocity * timeDelta; // normalize game speed so performance feels the same on all computers
    float theta = (angle - 90.0f) * (3.1415926f / 180.0f);
    deltaX = moveAmount * std::cos(theta);
    deltaY = moveAmount * std::sin(theta);

    detectSideCollision();
    detectPaddleCollision(1, paddle1);
    detectPaddleCollision(2, paddle2);
    if (!isGameOver()) {
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            frameIndex = (frameIndex + 1) % numFrames; // animate the game ball
            clock.restart();
        }
        sprite.move(deltaX, deltaY); // update the ball's position
    }
}

// ricochet off the side of the screen
void GameBall::detectSideCollision() {
    if (getLeft() + deltaX < 0 || getRight() + deltaX > Game::SCREEN_WIDTH) {
        deltaX = -deltaX; // bounce in the other direction
        angle = 360.0f - angle;
    }
}

// bounce off the paddle
void GameBall::detectPaddleCollision(int player, PlayerPaddle* paddle) {
    if (paddle->getGlobalBounds().intersects(this->getGlobalBounds())) {
        velocity += 25.0f; // every time the ball hits a paddle, it speeds up
        deltaY = -deltaY;  // reverse vertical direction
        fixOverlap(player, paddle);
        updateScore(player, paddle, 10);
        addEnglish(paddle->getVelocity());
        reflectAngle(); // bounce off the paddle
        ServiceManager::Audio()->playSound("audio/blip.wav");
    }
}

// if the player misses, game over
bool GameBall::isGameOver() {
    // the AI can't ever lose - bounce off the top of the screen even if it misses
    if (getTop() <= 0) {
        angle = 180.0f - angle;
        deltaY = -deltaY;
    }
    if (getBottom() + deltaY >= Game::SCREEN_HEIGHT) {
        ServiceManager::Audio()->playSound("audio/kaboom.wav");
        game->setStateGameOver();
        return true;
    } else
        return false;
}

// Make sure ball isn't inside paddle
void GameBall::fixOverlap(int player, PlayerPaddle* paddle) {
    if (player == 1) { // player 1's paddle is on the bottom of the screen
        if (getBottom() > paddle->getTop())
            setPosition(getPosition().x, paddle->getTop() - getWidth() / 2 - 1);
    } else { // player 2's paddle is on the top of the screen
        if (getTop() < paddle->getBottom())
            setPosition(getPosition().x, paddle->getBottom() + getWidth() / 2 + 1);
    }
}

// Update the paddle/player score display
void GameBall::updateScore(int player, PlayerPaddle* paddle, int points) {
    paddle->addPoints(points);
    std::stringstream name, text;
    name << "Score" << player;
    text << "Player " << player << " score: " << paddle->getScore();
    GameText* scoreText = game->getPlayObject<GameText>(name.str());
    scoreText->setText(text.str());
}

// add "English" based on the paddle's movement direction
void GameBall::addEnglish(const float speed) {
    if (speed < 0) {    // moving left
        angle -= 10.0f; // angle more to the left
        if (angle < 0) angle = 360.0f - angle;
    } else if (speed > 0) {
        angle += 10.0f;
        if (angle > 360.0f) angle = angle - 360.0f;
    }
}

// reflect around the vertical axis
void GameBall::reflectAngle() {
    angle = 360.0f - (angle - 180.0f);
    if (angle > 360.0f) angle -= 360.0f;
}
