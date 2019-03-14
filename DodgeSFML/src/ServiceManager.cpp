#include "pch.h"
#include "ServiceManager.h"

// Things to remember when using classes with static variables & methods:
// * static variables need to be initialized outside of the class
// * static variables cannot be set inside a class constructor
// * static variables are initialized at runtime
// * static methods can only access static member variables
// * static methods do not have a "this" pointer
IAudioProvider* ServiceManager::audioProvider;

IAudioProvider* ServiceManager::Audio() {
    return audioProvider;
}

void ServiceManager::registerAudioProvider(IAudioProvider* provider) {
    audioProvider = provider;
} 
