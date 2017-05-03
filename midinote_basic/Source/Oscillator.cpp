/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Apr 2017 7:46:58pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "Oscillator.h"

WaveSound::WaveSound (double lev, double leftRight, Wave w)
: level (lev), 
  pan (leftRight),
  wave (w)
{
}

bool WaveSound::appliesToNote (int midiNoteNumber)
{
    return true;
}
bool WaveSound::appliesToChannel (int midiChannel)
{
    return true;
}


Oscillator::Oscillator() : wave (WaveSound::sine),
                           currentAngle (0),
                           angleDelta (0),
                           currentFrequency (0),
                           currentSampleSize (0),
                           cyclesPerSample(0),
                           currentTime (0),
                           baseLevel (0.125),
                           level (0),
                           frequencyA4 (440.0),
                           pulseWidth (0),
                           tailOff (0),
                           pan (0)
{
}

bool Oscillator::canPlaySound (SynthesiserSound* sound)
{
    return (dynamic_cast<WaveSound*> (sound) != nullptr);
}

void Oscillator::startNote (int midiNoteNumber, float velocity,
                            SynthesiserSound* sound, int /*currentPitchWheelPosition*/)
{
    WaveSound* waveSound = dynamic_cast<WaveSound*>(sound);
    currentAngle = 0.0;
    currentTime = 0.0;
    currentFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    currentSampleSize = 1 / getSampleRate();
    cyclesPerSample = currentFrequency * currentSampleSize;
    angleDelta = currentSampleSize * 2.0 * double_Pi;
    
    level = velocity * baseLevel * waveSound->level;
    tailOff = 0.0;
    wave = waveSound->wave;
    pan = waveSound->pan;
}

void Oscillator::stopNote (float /*Velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (tailOff == 0.0) tailOff = 1.0;
    }
    else
    {
        clearCurrentNote();
        angleDelta = 0.0;
        currentTime = 0.0;
    }
}

void Oscillator::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (angleDelta != 0.0)
    {
        int numChannels = outputBuffer.getNumChannels();
        if (tailOff > 0)
        {
            while (--numSamples > 0) 
            {
                if (numChannels == 2)
                {
                    const std::pair<float,float> currentSample = applyPan (pan, waveFunction());
                    outputBuffer.addSample (0, startSample, currentSample.first * tailOff);
                    outputBuffer.addSample (1, startSample, currentSample.second * tailOff);
                }
                else
                {
                    const float currentSample = waveFunction() * tailOff;
                    for (int i = 0; i < numChannels; ++i)
                        outputBuffer.addSample (i, startSample, currentSample);
                }
                
                currentAngle += angleDelta;
                currentTime += currentSampleSize;
                
                ++startSample;
                tailOff *= 0.99;
                if (tailOff < 0.005)
                {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    currentTime = 0.0;
                    break;
                }
            }
        }
        else
        {
            while (--numSamples > 0)
            {
                if (numChannels == 2)
                {
                    const std::pair<float,float> currentSample = applyPan (pan, waveFunction());
                    outputBuffer.addSample (0, startSample, currentSample.first);
                    outputBuffer.addSample (1, startSample, currentSample.second); 
                }
                else
                {
                    const float currentSample = waveFunction();
                    for (int i = 0; i < numChannels; ++i)
                        outputBuffer.addSample (i, startSample, currentSample);
                }
                
                currentAngle += angleDelta;
                currentTime += currentSampleSize;
                
                ++startSample;
            }
        }
    }
}

float Oscillator::waveFunction ()
{
    float outputSample;
    switch (wave)
    {
        case WaveSound::sine     : outputSample = sineWaveFunction();
                                   break;
        case WaveSound::square   : outputSample = squareWaveFunction();
                                   break;
        case WaveSound::triangle : outputSample = triangleWaveFunction();
                                   break;
        case WaveSound::sawtooth : outputSample = sawtoothWaveFunction();
                                   break;
        case WaveSound::noise    : outputSample = noiseFunction();
                                   break;
    }
    return outputSample;
}

float Oscillator::squareWaveFunction ()
{
    if (sin (currentAngle) - pulseWidth < 0)
        return 0.0 - level;
    else
        return 0.0 + level;
}

float Oscillator::sineWaveFunction ()
{
    return (float) sin (currentAngle) * level;
}

float Oscillator::triangleWaveFunction ()
{
    return 2 * fabsf(sawtoothWaveFunction()) - 1;
}

float Oscillator::sawtoothWaveFunction ()
{
    double tf = currentTime * currentFrequency;
    return 2 * (tf - floor(0.5 + tf)) * level;
}

float Oscillator::noiseFunction ()
{
    return random.nextFloat() * (2 * level) - level;
}
