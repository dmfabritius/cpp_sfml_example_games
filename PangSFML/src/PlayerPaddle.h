#pragma once
#include "GameSprite.h"

class PlayerPaddle : public GameSprite {
protected:
    float velocity;
    float maxVelocity;
    int score = 0;
    void detectSideCollision();

public:
    PlayerPaddle() = delete; // prevent construction without initial position
    PlayerPaddle(Game* game, const float x, const float y, const float velocity = 0, const float maxVelocity = 750);

    void update(const float elapsedTime);
    virtual void updateVelocity();
    float getVelocity() const;
    int getScore() const;
    int resetScore();
    void addPoints(const int points);
};