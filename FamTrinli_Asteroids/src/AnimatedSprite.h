#pragma once

class AnimatedSprite {
public:
    std::vector<sf::IntRect> frames;
    sf::Sprite sprite;
    float frame = 0; // the current frame of animation
    float rate; // used to control the rate of animation, generally used to slow it down
    float r; // the radius of the sprite; used during collection detection; 40% of the sprites width

    AnimatedSprite() {
        // default constructor necessary in order to declare the sprites hashtable in Game.h
    }
    AnimatedSprite(sf::Texture& texture, int x, int y, int w, int h, int count, float rate) :
        rate(rate), r(0.4f * w) {
        sprite.setTexture(texture);
        for (int i = 0; i < count; i++) // set up the rectangles that define the location of each frame in the texture
            frames.push_back(sf::IntRect(x + i * w, y, w, h));

        sprite.setOrigin(static_cast<float>(w * 0.5), static_cast<float>(h * 0.5)); // move the origin to the center
        sprite.setTextureRect(frames[0]); // set the sprite to show the first frame
    }
    void nextFrame() {
        frame += rate; // advance the frame (may not move it fully to the next frame)
        if (frame >= frames.size()) frame = 0; // loop the animation back to the first frame
        sprite.setTextureRect(frames[(int)frame]); // set the frame of animation
    }
    bool completed() {
        return frame + rate >= frames.size(); // set to true when the end of the animation cycle is reached
    }
};