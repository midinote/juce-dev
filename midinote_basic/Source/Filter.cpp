/*
  ==============================================================================

    Filter.cpp
    Created: 10 Feb 2018 4:01:45pm
    Author:  jimi

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter (Font labelFont, Justification labelJustification,
                Filter::dBMode _dB, float _cutoff, float _resonance)
:   RadialButtons (Filter::off, Filter::MODE_SIZE, labelFont, labelJustification,
                   {"OFF", "Lowpass", "Highpass", "Bandpass"})
{
    this->dB = _dB;
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

Filter::dBMode Filter::getdB()
{
    return dB;
}
void Filter::setdB (Filter::dBMode _dB)
{
    this->dB = _dB;
}
