/*
  ==============================================================================

    AudioDeviceManagement.h
    Created: 6 May 2017 2:56:48pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


AudioDeviceManager& getSharedAudioDeviceManager();
static void runtimePermissionsCallback (bool wasGranted);

static ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager;
