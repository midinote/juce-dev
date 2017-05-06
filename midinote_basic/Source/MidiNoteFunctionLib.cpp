/*
  ==============================================================================

    MidiNoteFunctionLib.cpp
    Created: 5 May 2017 9:37:23pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "MidiNoteFunctionLib.h"

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
