/*
  ==============================================================================

    MidiNoteFunctionLib.cpp
    Created: 7 Apr 2017 7:22:00pm
    Author:  Malcolm Flint

  ==============================================================================
*/
#ifndef LIBRARY_FUNCTIONS
#define LIBRARY_FUNCTIONS
#include "../JuceLibraryCode/JuceHeader.h"

float dBToVolume (float dB)
{
    return 0.125f * powf(10.0f, 0.05f*dB);
}

float volumeToDB (float volume)
{
    return 20.0f * log10f(8.0f * volume);
}

std::pair<float,float> applyPan (float pan, float level)
{
    float left = level * (0.5 - pan / 2);
    float right = level * (0.5 + pan / 2);
    return std::pair<float,float> (left, right);
}

#endif
