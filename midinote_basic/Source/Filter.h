/*
  ==============================================================================

    Filter.h
    Created: 10 Feb 2018 4:01:45pm
    Author:  jimi

  ==============================================================================
*/

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include "RadialButtons.h"

class Filter   : public RadialButtons
{
public:
    // Linux's GNU g++ interprets this as an unsigned int, but Juce::var only defines the conversion operator for signed int
    enum Mode : signed int {low = 0, high, band, MODE_SIZE};

    Filter (Font labelFont, Justification labelJustification,
            float cutoff = 2000, float resonance = -3);
    ~Filter();

    Mode getMode();

    float cutoff;
    float resonance;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};

#endif  // FILTER_H_INCLUDED
