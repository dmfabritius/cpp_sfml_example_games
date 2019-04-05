#pragma once

class Paddle : public Entity {
public:
    Paddle(sf::Sprite& sprite) :
        Entity(PADDLE, sprite, DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - 10) {
    }
    void update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x += 6.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x -= 6.0f;
        // don't go beyond the edge of the display
        if (x < 0) x = 0; else if (x + w > DISPLAY_WIDTH) x = DISPLAY_WIDTH - w;
    }
};