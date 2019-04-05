#pragma once

class Ball : public Entity {
public:
    Ball(sf::Sprite& sprite) :
        Entity(BALL, sprite, DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - 175.0f) {
        dx = rand() % 2 + 2.0f;
        if (rand() % 2) dx = -dx; // 50-50 chance to start out going left or right
        dy = 3;
    }
    void update() {
        x += dx;
        y += dy;
        if (x < 0 || x + w > DISPLAY_WIDTH)  dx = -dx; // bounce off the walls
        if (y < 0 || y + h > DISPLAY_HEIGHT)  dy = -dy; // bounce off the floor & ceiling
    }
};