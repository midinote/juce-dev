/*
  ==============================================================================

    MidiNoteFunctionLib.cpp
    Created: 7 Apr 2017 7:22:00pm
    Author:  Malcolm Flint

  ==============================================================================
*/
#ifndef MIDINOTEFUNCTION_H_INCLUDED
#define MIDINOTEFUNCTION_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

float dBToVolume (float dB);

float volumeToDB (float volume);

std::pair<float,float> applyPan (float pan, float level);

#endif
