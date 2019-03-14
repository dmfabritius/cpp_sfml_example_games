#pragma once
#include "GameObject.h"

class ObjectManager {
private:
    std::map<std::string, GameObject*> gameObjects;
    sf::Clock clock;

public:
    ~ObjectManager();

    GameObject* find(std::string name) const;
    void add(std::string name, GameObject* gameObject);
    void remove(std::string name);
    void clear();
    int size() const;

    void enter();
    void leave();
    void update();
    void draw();
};