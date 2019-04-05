#pragma once

class Laser : public Entity {
public:
    Laser(AnimatedSprite& anim, float x, float y, float angle) :
        Entity(LASER, anim, x, y, angle) {
        dx = 6 * cos(angle * DEG2RAD);
        dy = 6 * sin(angle * DEG2RAD);
    }
    void  update() {
        x += dx; // update the position
        y += dy;
        if (x < 0 || x > DISPLAY_WIDTH || y < 0 || y > DISPLAY_HEIGHT)
            active = false; // delete the laser pulse when it goes off the screen
    }
};
