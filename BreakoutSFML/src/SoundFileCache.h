#pragma once
#include "pch.h"

class SoundFileCache {
private:
    std::unordered_map<std::string, sf::Sound*> sounds;
    std::unordered_map<std::string, sf::Music*> songs;

public:
    ~SoundFileCache();

    sf::Sound* loadSound(const std::string& filename);
    sf::Sound* getSound(const std::string& filename);
    sf::Music* loadSong(const std::string& filename);
    sf::Music* getSong(const std::string& filename);
};
