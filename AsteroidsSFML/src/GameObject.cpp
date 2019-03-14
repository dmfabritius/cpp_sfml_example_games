#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(Game* game) :
    game(game), active(true), destroyed(false) {
}

bool GameObject::isActive() {
    return active;
}

bool GameObject::isDestroyed() {
    return destroyed;
}
