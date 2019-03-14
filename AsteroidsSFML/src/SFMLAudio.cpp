#include "pch.h"
#include "SFMLAudio.h"
#include "SoundFileCache.h"

void SFMLAudio::loadSound(std::string filename) {
    soundFileCache.loadSound(filename);
}

void SFMLAudio::playSound(std::string filename, bool overlap) {
    // look for an available channel to play a sound
    // if no channel is available, the loop ends and nothing happens (the sound is not played)
    for (sf::Sound* sound : currentSounds) {
        if (!sound || sound->getStatus() != sf::Sound::Playing) {
            sound = soundFileCache.getSound(filename);
            if (overlap || sound->getStatus() != sf::Sound::Playing) sound->play();
            break;
        }
    }
}

void SFMLAudio::loadSong(std::string filename) {
    soundFileCache.loadSong(filename);
}

void SFMLAudio::playSong(std::string filename, bool loop) {
    if (currentSong == filename) return; // already playing the desired song
    sf::Music* music;
    if (currentSong != "") { // stop any currently playing song
        music = soundFileCache.getSong(currentSong);
        music->stop();
    }
    music = soundFileCache.getSong(filename);
    currentSong = filename;
    music->setLoop(loop);
    music->setVolume(25); // lower volume to 25%
    music->play();
}

void SFMLAudio::stopAll() {
    for (sf::Sound* sound : currentSounds) {
        if (sound) sound->stop();
    }
    if (currentSong != "") {
        sf::Music* music = soundFileCache.getSong(currentSong);
        music->stop();
    }
}

bool SFMLAudio::isSoundPlaying() {
    for (sf::Sound* sound : currentSounds) {
        if (sound && sound->getStatus() == sf::Sound::Playing) return true;
    }
    return false;
}

bool SFMLAudio::isSongPlaying() {
    if (currentSong != "") {
        return soundFileCache.getSong(currentSong)->getStatus() == sf::Music::Playing;
    }
    return false;
}