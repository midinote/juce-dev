/*
  ==============================================================================

    MidiNoteFunctionLib.cpp
    Created: 7 Apr 2017 7:22:00pm
    Author:  Malcolm Flint

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#include "Filter.h"

float dBToVolume (float dB)
{
    return 0.125f * powf(10.0f, 0.05f*dB);
}

float volumeToDB (float volume)
{
    return 20.0f * log10f(8.0f * volume);
}

std::pair<float,float> applyPanAndFilter (float pan, float level, Filter* filter, double sampleRate)
{
    // the way JUCE's IIRFilter library is designed, it has its own off-switch (makeInactive())
    switch (filter->getMode()) {
        case Filter::low:
            filter->left.setCoefficients(IIRCoefficients::makeLowPass (sampleRate,
                                         filter->cutoff, filter->resonance));
            filter->right.setCoefficients(IIRCoefficients::makeLowPass (sampleRate,
                                          filter->cutoff, filter->resonance));
            break;
        case Filter::high:
            filter->left.setCoefficients(IIRCoefficients::makeHighPass (sampleRate,
                                         filter->cutoff, filter->resonance));
            filter->right.setCoefficients(IIRCoefficients::makeHighPass (sampleRate,
                                          filter->cutoff, filter->resonance));
            break;
        case Filter::band:
            filter->left.setCoefficients(IIRCoefficients::makeBandPass (sampleRate,
                                         filter->cutoff, filter->resonance));
            filter->right.setCoefficients(IIRCoefficients::makeBandPass (sampleRate,
                                          filter->cutoff, filter->resonance));
            break;
        default:
            filter->left.makeInactive();
            filter->right.makeInactive();
            break;
    }
    float left = level * (0.5 - pan);
    float right = level * (0.5 + pan);
    filter->left.processSamples (&left, 1);
    filter->right.processSamples (&right, 1);
    return std::pair<float,float> (left, right);
}
