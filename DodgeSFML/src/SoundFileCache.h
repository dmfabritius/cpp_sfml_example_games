#pragma once
#include "pch.h"

class SoundFileCache {
private:
    std::map<std::string, sf::Sound*> sounds;
    std::map<std::string, sf::Music*> songs;

public:
    ~SoundFileCache();

    sf::Sound* loadSound(const std::string& filename);
    sf::Sound* getSound(const std::string& filename);
    sf::Music* loadSong(const std::string& filename);
    sf::Music* getSong(const std::string& filename);
};
