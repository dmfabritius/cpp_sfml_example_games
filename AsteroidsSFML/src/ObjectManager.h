#pragma once
#include "pch.h"
#include "GameObject.h"

class ObjectManager {
private:
    std::map<std::string, GameObject*> gameObjects;
    sf::Clock clock;
    int uniqueID = 0;

public:
    ~ObjectManager();

    GameObject* find(std::string name) const;
    void add(GameObject* gameObject, std::string name = "");
    void remove(std::string name);
    void remove(GameObject* gameObject);
    void clear();
    int size() const;

    void enter();
    void leave();
    void update();
    void draw();
};