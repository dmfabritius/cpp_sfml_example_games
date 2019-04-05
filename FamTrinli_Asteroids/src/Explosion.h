#pragma once

class Explosion : public Entity {
public:
    Explosion(AnimatedSprite& anim, float x, float y) :
        Entity(EXPLOSION, anim, x, y) {
    }
    void update() {
        active = !anim.completed(); // remove when the animation completes
    }
};
