/*
  ==============================================================================

    Filter.cpp
    Created: 10 Feb 2018 4:01:45pm
    Author:  jimi

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter (Font labelFont, Justification labelJustification, float _cutoff, float _resonance)
:   RadialButtons (Filter::off, Filter::MODE_SIZE, labelFont, labelJustification,
                   {"OFF", "Lowpass", "Highpass", "Bandpass"})
{
    this->cutoff = _cutoff;
    this->resonance = _resonance;
}

Filter::~Filter()
{
}

Filter::Mode Filter::getMode()
{
    return static_cast<Filter::Mode> (getValue());
}
