#include "pch.h"
#include "Game.h"
#include "ServiceManager.h"
#include "GameText.h"
#include "GetReady.h"
#include "GameBall.h"
#include "AIPaddle.h"

GameBall::GameBall(Game* game) :
    GameSprite(game, "images/balls.png", 8),
    scoreText(game->getPlayObject<GameText>("Score")),
    getReady(game->getPlayObject<GetReady>("GetReady")) {
    startingPosition.x = (Game::SCREEN_WIDTH - getWidth()) * 0.5f;
    startingPosition.y = (Game::SCREEN_HEIGHT - getHeight()) * 0.5f;
}

void GameBall::enter() {
    setPosition(startingPosition);
    gameSpeed = 250.0f;
    angle = (float)(rand() % 90 + 135);
}

void GameBall::update(float timeDelta) {
    if (getReady->isActive()) return;   // wait for "get ready" message to complete before moving the ball
    timeDelta = std::min(timeDelta, 0.05f);  // prevent huge time gaps
    float moveAmount = gameSpeed * timeDelta; // normalize game speed so performance feels the same on all computers
    float theta = (angle - 90.0f) * (3.1415926f / 180.0f);
    velocity.x = moveAmount * std::cos(theta);
    velocity.y = moveAmount * std::sin(theta);

    detectSideCollision();
    detectPaddleCollision();

    if (!isGameOver()) {
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            //sprite.rotate(15); // because the sprite is a square, rotating it changes its width and height, messing up our collision detection
            frameIndex = (frameIndex + 1) % numFrames; // animate the game ball
            clock.restart();
        }
        sprite.move(velocity); // update the ball's position
    }
}

// ricochet off the side of the screen
void GameBall::detectSideCollision() {
    if (getLeft() + velocity.x < 0 || getRight() + velocity.x > Game::SCREEN_WIDTH) {
        velocity.x = -velocity.x; // bounce in the other direction
        angle = 360.0f - angle;
    }
}

// bounce off the paddle
void GameBall::detectPaddleCollision() {
    if (game->getPaddle()->getGlobalBounds().intersects(this->getGlobalBounds())) {
        if (game->isAllBricksDestroyed()) game->createBricks();
        fixOverlap(game->getPaddle());
        gameSpeed += 10.0f; // every time the ball hits a paddle, it speeds up
        velocity.y = -velocity.y;  // reverse vertical direction
        addEnglish(game->getPaddle()->getVelocity());
        reflectAngle(); // bounce off the paddle
        ServiceManager::Audio()->playSound("audio/blip.wav");
    }
}

// if the player misses, game over
bool GameBall::isGameOver() {
    // bounce off the top of the screen
    if (getTop() <= 0) {
        angle = 180.0f - angle;
        velocity.y = -velocity.y;  // reverse vertical direction
    }

    if (getBottom() + velocity.y >= Game::SCREEN_HEIGHT) {
        ServiceManager::Audio()->playSound("audio/kaboom.wav");
        game->setStateGameOver();
        return true;
    } else
        return false;
}

// Make sure ball isn't inside another object
void GameBall::fixOverlap(GameSprite* other) {
    if (velocity.y > 0) { // ball is moving downwards
        if (getBottom() > other->getTop())
            setPosition(getPosition().x, other->getTop() - getHeight() * 0.5f - 2);
    } else { // ball is moving upwards
        if (getTop() < other->getBottom())
            setPosition(getPosition().x, other->getBottom() + getHeight() * 0.5f + 2);
    }
}

// add "English" based on the paddle's movement direction
void GameBall::addEnglish(const float speed) {
    if (speed == 0) return; // don't add "English" if the paddle isn't moving
    if (angle < 90) { // ball is moving to the right and down
        if (speed > 0) { // paddle is moving right
            angle += 3; // angle even more to the right
            angle = std::min(70.0f, angle); // don't lean too far to the right
        } else
            angle -= 3; // angle more to the left
    } else if (angle < 180) { // ball is moving to the right and up
        if (speed > 0) { // paddle is moving right
            angle -= 3; // angle even more to the right
            angle = std::max(angle, 120.0f); // don't lean too far to the right
        } else
            angle += 3; // angle more to the left
    } else if (angle < 270) { // ball is moving to the left and up
        if (speed > 0) // paddle is moving right
            angle -= 3; // angle more to the right
        else {
            angle += 3; // angle even more to the left
            angle = std::min(250.0f, angle); // don't lean too far to the left
        }
    } else { // ball is moving to the left and down
        if (speed > 0) // paddle is moving right
            angle += 3; // angle more to the right
        else {
            angle -= 3; // angle even more to the left
            angle = std::max(angle, 290.0f);
        }
    }
}

// reflect around the vertical axis
void GameBall::reflectAngle() {
    angle = 360.0f - (angle - 180.0f);
    if (angle > 360.0f) angle -= 360.0f;
}
