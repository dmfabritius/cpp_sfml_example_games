#pragma once
#include "IAudioProvider.h"
#include "SoundFileCache.h"

class SFMLSoundProvider : public IAudioProvider {
private:
    SoundFileCache soundFileCache;
    sf::Sound* currentSounds[5] = { nullptr }; // allow up to 5 sound effects to play at once
    std::string currentSong;

public:
    void loadSound(std::string filename);
    void playSound(std::string filename, bool overlap = false);
    void loadSong(std::string filename);
    void playSong(std::string filename, bool looping = false);
    void stopAll();

    bool isSoundPlaying();
    bool isSongPlaying();
};