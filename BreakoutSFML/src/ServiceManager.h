#pragma once
#include "pch.h"
#include "IAudioProvider.h"

class ServiceManager {
private:
    static IAudioProvider* audioProvider;

public:
    static IAudioProvider* Audio();
    static void registerAudioProvider(IAudioProvider* provider);
};