#pragma once

class Asteroid : public Entity {
public:
    Asteroid(AnimatedSprite& anim, float x, float y) :
        Entity(ASTEROID, anim, x, y, randf(360)) {
        dx = 1 + randf(2); // move at a random speed
        dy = 1 + randf(2);
        if (std::rand() % 2) dx *= -1; // move to the left or right
        if (std::rand() % 2) dy *= -1;
    }
    void update() {
        x += dx; // update the position with its delta
        y += dy;
        if (x < 0) x = DISPLAY_WIDTH; else if (x > DISPLAY_WIDTH) x = 0;
        if (y < 0) y = DISPLAY_HEIGHT; else if (y > DISPLAY_HEIGHT) y = 0;
    }
};
