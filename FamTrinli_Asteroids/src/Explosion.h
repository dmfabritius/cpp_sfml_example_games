#pragma once

class Explosion : public Entity {
public:
    Explosion(AnimatedSprite& anim, float x, float y) :
        Entity("explosion", anim, x, y) { // this class was created just to be consistent with the others
    }
};
