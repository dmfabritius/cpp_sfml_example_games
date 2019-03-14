#include "pch.h"
#include "SoundFileCache.h"

SoundFileCache::~SoundFileCache(void) {
    for (const auto& sound : sounds) { // sounds is a map, each sound is a pair<string, buffer*>
        delete sound.second;           // the second element of the sound pair is a pointer to a buffer
    }
    for (const auto& song : songs) {
        delete song.second;
    }
}

sf::Sound* SoundFileCache::loadSound(const std::string& filename) {
    sf::Sound* sound = new sf::Sound();
    sf::SoundBuffer* buffer = new sf::SoundBuffer();
    if (buffer->loadFromFile(filename)) {
        sounds.insert(std::pair<std::string, sf::Sound*>(filename, sound));
        sound->setBuffer(*buffer);
    } else {
        delete buffer;
        delete sound;
        throw std::runtime_error(filename + " was not found in SoundFileCache::loadSound");
    }
    return sound;
}

sf::Sound* SoundFileCache::getSound(const std::string& filename) {
    sf::Sound* sound;
    auto it = sounds.find(filename);
    if (it == sounds.end()) {
        sound = loadSound(filename); // sound was not found in cache, so load it from file
    } else {
        sound = it->second; // sound was found in cache (the sound pointer is the second element)
    }
    return sound;
}

sf::Music* SoundFileCache::loadSong(const std::string& filename) {
    sf::Music* music = new sf::Music();
    if (music->openFromFile(filename)) {
        songs.insert(std::pair<std::string, sf::Music*>(filename, music));
    } else {
        delete music;
        throw std::runtime_error(filename + " was not found in call to SoundFileCache::loadSong");
    }
    return music;
}

sf::Music* SoundFileCache::getSong(const std::string& filename) {
    sf::Music* music;
    auto it = songs.find(filename);
    if (it == songs.end()) {
        music = loadSong(filename); // song was not found in cache, so load it from file
    } else {
        music = it->second; // song was found in cache (the music pointer is the second element)
    }
    return music;
}
