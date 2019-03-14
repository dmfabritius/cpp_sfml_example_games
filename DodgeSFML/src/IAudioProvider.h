#pragma once
#include "pch.h"

class IAudioProvider {
public:
    virtual void loadSound(std::string filename) = 0;
    virtual void playSound(std::string filename, bool overlap = false) = 0;
    virtual void loadSong(std::string filename) = 0;
    virtual void playSong(std::string filename, bool looping = false) = 0;
    virtual void stopAll() = 0;

    virtual bool isSoundPlaying() = 0;
    virtual bool isSongPlaying() = 0;
};