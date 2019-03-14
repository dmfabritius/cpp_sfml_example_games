#pragma once
#include "GameSprite.h"
#include "PlayerPaddle.h"
#include "GameOver.h"

class GameBall : public GameSprite {
private:
    PlayerPaddle *paddle1, *paddle2;
    GameText* getReady;
    sf::Vector2f startingPosition;
    float velocity;
    float deltaX, deltaY;
    float angle;
    sf::Clock clock; // used to change sprite frames and animate the ball

public:
    GameBall() = delete; // prevent construction without initial position
    GameBall(Game* game);
    void enter(); // runs when the game state that this object belongs to becomes active
    void update(float timeDelta);

private:
    void countdown(int milliseconds);
    void detectSideCollision();
    void detectPaddleCollision(int player, PlayerPaddle* paddle);
    bool isGameOver();
    void fixOverlap(int player, PlayerPaddle* paddle);
    void updateScore(int player, PlayerPaddle* paddle, int points);
    void addEnglish(const float speed);
    void reflectAngle();
};
