#pragma once
#include "pch.h"
#include "IAudioProvider.h"
#include "SoundFileCache.h"

class SFMLSoundProvider : public IAudioProvider {
private:
    SoundFileCache soundFileCache;
    sf::Sound* currentSounds[5] = { 0 }; // allow up to 5 sound effects to play at once
    std::string currentSong;

public:
    void loadSound(std::string filename);
    void playSound(std::string filename);
    void loadSong(std::string filename);
    void playSong(std::string filename, bool looping = false);
    void stopAll();

    bool isSoundPlaying();
    bool isSongPlaying();
};