/*
  ==============================================================================

    Oscillator.h
    Created: 7 Apr 2017 7:46:58pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include <typeinfo>
#include "../JuceLibraryCode/JuceHeader.h"

class SquareWave : SynthesiserSound
{
    SquareWave ();
    bool appliesToNote (int midiNoteNumber);
    bool appliesToChannel (int midiChannel);
};

class SineWave : SynthesiserSound
{
    SineWave ();
    bool appliesToNote (int midiNoteNumber);
    bool appliesToChannel (int midiChannel);
};

class SawtoothWave : SynthesiserSound
{
    SawtoothWave ();
    bool appliesToNote (int midiNoteNumber);
    bool appliesToChannel (int midiChannel);
};

class TriangleWave : SynthesiserSound
{
    TriangleWave();
    bool appliesToNote (int midiNoteNumber);
    bool appliesToChannel (int midiChannel);
};

class Noise : SynthesiserSound
{
    Noise ();
    bool appliesToNote (int midiNoteNumber);
    bool appliesToChannel (int midiChannel);
};

class Oscillator : public SynthesizerVoice
{
    Oscillator()
}

#endif  // OSCILLATOR_H_INCLUDED
