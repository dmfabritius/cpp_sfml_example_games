#pragma once
#include "GameSprite.h"
#include "PlayerPaddle.h"
#include "GameOver.h"

class GameBall : public GameSprite {
private:
    GameText* getReady;
    GameText* scoreText;
    sf::Vector2f startingPosition;
    sf::Vector2f velocity;
    float gameSpeed;
    float angle;
    sf::Clock clock; // used to change sprite frames and animate the ball

public:
    GameBall() = delete; // prevent construction without initial position
    GameBall(Game* game);
    void enter(); // runs when the game state that this object belongs to becomes active
    void update(float timeDelta);
    void reflectAngle();
    void fixOverlap(GameSprite* sprite);

private:
    void detectSideCollision();
    void detectPaddleCollision();
    bool isGameOver();
    void addEnglish(const float speed);
};
