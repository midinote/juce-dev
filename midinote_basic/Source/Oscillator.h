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
#include "MidiNoteFunctionLib.h"

class WaveSound : public SynthesiserSound
{
public:
    enum Wave {square, sine, sawtooth, triangle, noise};
    
    WaveSound (double lev, double leftRight, Wave w);
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
    double level, pan;
    Wave wave;
};

class Oscillator : public SynthesiserVoice
{
public:
    Oscillator();
    
    bool canPlaySound (SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* sound, int /*currentPitchWheelPosition*/) override;
    void stopNote (float /*Velocity*/, bool allowTailOff) override;
    void pitchWheelMoved (int /*newValue*/) override;
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override;
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    
    float waveFunction ();
    float squareWaveFunction ();
    float sineWaveFunction ();
    float sawtoothWaveFunction ();
    float triangleWaveFunction ();
    float noiseFunction ();
private:
    Random random;
    WaveSound::Wave wave;
    double currentAngle, angleDelta, currentFrequency, currentSampleSize, cyclesPerSample,
           currentTime, baseLevel, level, frequencyA4, pulseWidth, tailOff, pan;
};

#endif  // OSCILLATOR_H_INCLUDED
