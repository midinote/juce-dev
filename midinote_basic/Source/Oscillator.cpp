/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Apr 2017 7:46:58pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "Oscillator.h"


bool SquareWave::appliesToNote (int midiNoteNumber)
{
    return true;
}
bool SquareWave::appliesToChannel (int midiChannel)
{
    return true;
}

bool SineWave::appliesToNote (int midiNoteNumber)
{
    return true;
}
bool SineWave::appliesToChannel (int midiChannel)
{
    return true;
}

bool SawtoothWave::appliesToNote (int midiNoteNumber)
{
    return true;
}
bool SawtoothWave::appliesToChannel (int midiChannel)
{
    return true;
}

bool TriangleWave::appliesToNote (int midiNoteNumber)
{
    return true;
}
bool TriangleWave::appliesToChannel (int midiChannel)
{
    return true;
}

bool Noise::appliesToNote (int midiNoteNumber)
{
    return true;
}
bool Noise::appliesToChannel (int midiChannel)
{
    return true;
}


