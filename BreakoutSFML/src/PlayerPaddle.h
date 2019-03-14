#pragma once
#include "GameSprite.h"

class PlayerPaddle : public GameSprite {
protected:
    float velocityX;
    float maxVelocityX;
    void detectSideCollision();

public:
    PlayerPaddle() = delete; // prevent construction without initial position
    PlayerPaddle(Game* game, const float x, const float y, const float velocityX = 0, const float maxVelocityX = 750);

    void update(const float elapsedTime);
    virtual void updateVelocity();
    float getVelocity() const;
};