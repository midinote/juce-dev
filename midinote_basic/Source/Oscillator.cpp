/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Apr 2017 7:46:58pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "Oscillator.h"

/*
 Oscillator class implementation
 */
Oscillator::Oscillator(float freq, float lev, WaveType waveType)
:   currentAngle (0.0),
    angleDelta (0.0),
    currentSampleSize (0.0)
{
    wave = waveType;
    frequencyA4 = freq;
    level = lev;
}

float Oscillator::oscillate (double sampleRate, double frequency)
{
    currentSampleSize = 1.0 / sampleRate;
    updateAngleDelta(frequency);
    currentFrequency = frequency;
    float outputSample;
    switch (wave)
    {
        case sine     : outputSample = sineWaveFunction();
                            break;
        case square   : outputSample = squareWaveFunction();
                            break;
        case triangle : outputSample = triangleWaveFunction();
                            break;
        case sawtooth : outputSample = sawtoothWaveFunction();
                            break;
        case noise    : outputSample = NoiseFunction();
                            break;
    }
    currentAngle += angleDelta;
    return outputSample;
}

Oscillator::WaveType Oscillator::getWaveType ()
{
    return wave;
}

float Oscillator::getTuning ()
{
    return frequencyA4;
}

float Oscillator::getLevel ()
{
    return level;
}

void Oscillator::setWave(Oscillator::WaveType waveType)
{
    wave = waveType;
}

void Oscillator::setTuning (float freq)
{
    frequencyA4 = freq;
}

void Oscillator::setLevel (float lev)
{
    level = lev;
}


void Oscillator::updateAngleDelta(double frequency)
{
    const double cyclesPerSample = frequency * currentSampleSize;
    angleDelta = cyclesPerSample * 2.0 * double_Pi;
}

void Oscillator::setPulseWidth(float pw)
{
    pulseWidth = pw;
}

float Oscillator::getPulseWidth()
{
    return pulseWidth;
}

float Oscillator::squareWaveFunction()
{
    if (sin (currentAngle) - pulseWidth < 0)
        return 0.0 - level;
    else
        return 0.0 + level;
}

float Oscillator::sineWaveFunction()
{
    return (float) sin (currentAngle) * level;
}

float Oscillator::triangleWaveFunction()
{
    return 2 * fabsf(sawtoothWaveFunction()) - 1;
}

float Oscillator::sawtoothWaveFunction()
{
    double anglePiF = currentAngle * double_Pi * currentFrequency;
    return 0.0 - 2.0 * level / double_Pi * atan(cos(anglePiF)/sin(anglePiF));
}

float Oscillator::NoiseFunction()
{
    return random.nextFloat() * (2 * level) - level;
}



