/*
  ==============================================================================

    AudioDeviceManagement.cpp
    Created: 6 May 2017 2:56:48pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "AudioDeviceManagement.h"

AudioDeviceManager& getSharedAudioDeviceManager()
{
    if (sharedAudioDeviceManager == nullptr)
    {
        sharedAudioDeviceManager = new AudioDeviceManager();
        RuntimePermissions::request (RuntimePermissions::recordAudio, runtimePermissionsCallback);
    }
    
    return *sharedAudioDeviceManager;
}

void runtimePermissionsCallback (bool wasGranted)
{
    int numInputChannels = wasGranted ? 2 : 0;
    sharedAudioDeviceManager->initialise (numInputChannels, 2, nullptr, true, String(), nullptr);
}
