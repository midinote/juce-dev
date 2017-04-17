/*
  ==============================================================================

    Synth.cpp
    Created: 7 Apr 2017 7:05:20pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "Synth.h"
#include "MidiNoteFunctionLib.h"

//==============================================================================
Synth::Synth()
:   waveType(Oscillator::sine),
    noteOn(false)
{    
    addAndMakeVisible (frequencySlider);
    frequencySlider.setRange (55.0, 14080.0);
    frequencySlider.setSkewFactorFromMidPoint (440.0);
    frequencySlider.setValue(440.0);
    frequencySlider.addListener(this);
    
    addAndMakeVisible(levelSlider);
    levelSlider.setRange (-100, 6.0);
    levelSlider.setSkewFactorFromMidPoint(-6.0);
    levelSlider.setValue(0.0);
    levelSlider.addListener(this);
    
    addAndMakeVisible(panSlider);
    panSlider.addListener(this);
}

Synth::~Synth()
{
}

void Synth::paint (Graphics& g)
{
}

float Synth::synthesize(double sampleRate)
{
    double sample = 0.0;
    for (std::map<int,std::pair<MidiMessage,Oscillator>>::iterator itor=currentNotes.begin();
         itor!=currentNotes.end(); ++itor)
    {
        currentFrequency = MidiMessage::getMidiNoteInHertz(itor->first, frequencySlider.getValue());
        sample += itor->second.second.oscillate(sampleRate, currentFrequency);
    }
    return sample;
}

void Synth::addNote (MidiMessage message)
{
    Oscillator osc (frequencySlider.getValue(), dBToVolume (levelSlider.getValue()), Oscillator::sine);
    std::pair<MidiMessage, Oscillator> val (message, osc);
    int key = message.getNoteNumber();
    currentNotes.insert (std::pair<int, std::pair<MidiMessage, Oscillator>> (key, val));
}

void Synth::removeNote (MidiMessage message)
{
    currentNotes.erase (message.getNoteNumber());
}

void Synth::resized()
{
    frequencySlider.setBounds (10, 10, getWidth() - 20, 20);
    levelSlider.setBounds (10, 20, getWidth() - 20, 40);
    panSlider.setBounds (10, 30, getWidth() - 20, 60);
}

void Synth::sliderValueChanged(Slider* slider)
{
}

