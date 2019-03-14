#include "pch.h"
#include "ObjectManager.h"
#include "Game.h"

ObjectManager::~ObjectManager() {
    for (const auto& obj : gameObjects) { // gameObjects is a map, each obj is a pair<string, GameObject*>
        delete obj.second;                // the second element of the obj pair is a pointer to a GameObject
    }
}

GameObject* ObjectManager::find(std::string name) const {
    std::map<std::string, GameObject*>::const_iterator obj = gameObjects.find(name);
    if (obj == gameObjects.end())
        throw std::runtime_error(name + " was not found in GameObjectManager::find");
    return obj->second;
}

void ObjectManager::add(std::string name, GameObject* gameObject) {
    //gameObjects.insert(std::pair<std::string, GameObject*>(name, gameObject));
    gameObjects[name] = gameObject;
}

void ObjectManager::remove(std::string name) {
    std::map<std::string, GameObject*>::iterator obj = gameObjects.find(name);
    if (obj != gameObjects.end()) {
        delete obj->second;     // deallocate the game object from the heap
        gameObjects.erase(obj); // remove it from the collection of active game objects
    }
}

void ObjectManager::clear() {
    auto obj = gameObjects.begin();
    while (obj != gameObjects.end()) {
        delete obj->second;           // deallocate the game object from the heap
        obj = gameObjects.erase(obj); // remove it from the collection of game objects & advance to the next one
    }
}

int ObjectManager::size() const {
    return gameObjects.size();
}

void ObjectManager::enter() {
    for (const auto& obj : gameObjects) {
        obj.second->enter();
    }
}

void ObjectManager::leave() {
    for (const auto& obj : gameObjects) {
        obj.second->leave();
    }
}

void ObjectManager::update() {
    float timeDelta = clock.restart().asSeconds(); // the amount of time since the last update cycle
    auto obj = gameObjects.begin();
    while (obj != gameObjects.end()) {
        if (obj->second->isDestroyed()) {
            delete obj->second;           // deallocate the game object from the heap
            obj = gameObjects.erase(obj); // remove it from the collection of game objects & advance to next object
        } else {
            obj->second->update(timeDelta);
            obj++;
        }
    }
}

void ObjectManager::draw() {
    for (const auto& obj : gameObjects) {
        obj.second->draw();
    }
}
