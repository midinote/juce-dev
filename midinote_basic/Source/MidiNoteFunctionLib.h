/*
  ==============================================================================

    MidiNoteFunctionLib.cpp
    Created: 7 Apr 2017 7:22:00pm
    Author:  Malcolm Flint

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

float dBToVolume (float dB)
{
    return 0.125f * powf(10.0f, 0.05f*dB);
}

float volumeToDB (float volume)
{
    return 8.0f * 20.0f * log10f(volume);
}

std::pair<float,float> applyPan (float pan, float level)
{
    float left = level * (0.5 - pan);
    float right = level * (0.5 + pan);
    return std::pair<float,float> (left, right);
}
