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
    enum Mode   : signed int {off = 0, low, high, band, MODE_SIZE};
    enum dBMode : signed int {dB6 = 0, dB12, dB18, dB24, DB_MODE_SIZE};

    Filter (Font labelFont, Justification labelJustification,
            dBMode dB = dB12, float cutoff = 2000, float resonance = 1);
    ~Filter();

    Mode getMode();
    void setMode (Mode mode);

    dBMode getdB();
    void setdB (dBMode dB);

    dBMode dB;
    float cutoff;
    float resonance;
    // separate left/right channel instances is advice from https://forum.juce.com/t/a-basic-code-on-how-to-add-the-iir-filter/23163/2
    IIRFilter left;
    IIRFilter right;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};

#endif  // FILTER_H_INCLUDED
