/*
  ==============================================================================

    Oscillator.h
    Created: 7 Apr 2017 7:46:58pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator
{
public:
    enum WaveType {sine, square, triangle, sawtooth, noise};

    Oscillator (float freq, float lev, WaveType waveType);
    
    float oscillate (double, double);
    
    WaveType getWaveType ();
    float getTuning ();
    float getLevel ();
    float getPulseWidth ();
    
    void setWave (WaveType);
    void setTuning (float);
    void setLevel (float);
    void setPulseWidth (float);

private:
    
    float squareWaveFunction ();
    float sineWaveFunction ();
    float triangleWaveFunction ();
    float sawtoothWaveFunction ();
    float NoiseFunction ();
    
    void updateAngleDelta (double);
    
    WaveType wave;
    double currentAngle, angleDelta, currentFrequency, currentSampleSize;
    float frequencyA4, level, pulseWidth;
    Random random;
};


#endif  // OSCILLATOR_H_INCLUDED
