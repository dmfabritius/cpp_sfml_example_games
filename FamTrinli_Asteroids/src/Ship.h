#pragma once

constexpr int maxSpeed = 15;

class Ship : public Entity {
public:
    bool thrusting;

    Ship(AnimatedSprite& anim, float x, float y, float angle = 0) :
        Entity(SHIP, anim, x, y, angle) {
    }
    void update() {
        if (thrusting) {
            dx += cos(angle * DEG2RAD) * 0.2f;
            dy += sin(angle * DEG2RAD) * 0.2f;
        } else {
            dx *= 0.99f;
            dy *= 0.99f;
        }

        float speed = sqrt(dx*dx + dy*dy);
        if (speed > maxSpeed) {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx; // update the position
        y += dy;
        if (x < 0) x = DISPLAY_WIDTH; else if (x > DISPLAY_WIDTH) x = 0;
        if (y < 0) y = DISPLAY_HEIGHT; else if (y > DISPLAY_HEIGHT) y = 0;
    }
};
