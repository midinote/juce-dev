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
    labelFont(Font (12)),
    labelJustification(Justification::centredTop)
{
    addAndMakeVisible (frequencySlider);
    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    frequencySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    frequencySlider.setRange (55.0, 14080.0);
    frequencySlider.setSkewFactorFromMidPoint (440.0);
    frequencySlider.setValue(440.0);
    frequencySlider.addListener(this);
    
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Coarse\nTuning", dontSendNotification);
    frequencyLabel.setFont(labelFont);
    frequencyLabel.setJustificationType(labelJustification);
    frequencyLabel.attachToComponent(&frequencySlider, false);
    
    addAndMakeVisible(levelSlider);
    levelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    levelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    levelSlider.setRange (-100, 6.0);
    levelSlider.setSkewFactorFromMidPoint(-6.0);
    levelSlider.setValue(0.0);
    levelSlider.addListener(this);
    
    addAndMakeVisible(levelLabel);
    levelLabel.setText("Level", dontSendNotification);
    levelLabel.setFont(labelFont);
    levelLabel.setJustificationType(labelJustification);
    levelLabel.attachToComponent(&levelSlider, false);
    
    addAndMakeVisible (waveSlider);
    waveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    waveSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    waveSlider.setRange (0.0, 5.0);
    waveSlider.setValue(0.0);
    waveSlider.addListener(this);
    
    addAndMakeVisible(waveLabel);
    waveLabel.setText("Wave", dontSendNotification);
    waveLabel.setFont(labelFont);
    waveLabel.setJustificationType(labelJustification);
    waveLabel.attachToComponent(&waveSlider, false);
    
    addAndMakeVisible(panSlider);
    panSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    panSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    panSlider.setRange(-0.5,0.5);
    panSlider.setValue(0.0);
    panSlider.addListener(this);
    
    addAndMakeVisible(panLabel);
    panLabel.setText("Pan", dontSendNotification);
    panLabel.setFont(labelFont);
    panLabel.setJustificationType(labelJustification);
    panLabel.attachToComponent(&panSlider, false);
    
    updateSettings(frequencySlider.getValue(),
                   static_cast<Oscillator::WaveType> (floorf(waveSlider.getValue())),
                   dBToVolume(levelSlider.getValue()),
                   panSlider.getValue());
}

Synth::~Synth()
{
}

void Synth::paint (Graphics& g)
{
}

std::pair<float,float> Synth::synthesize(double sampleRate)
{
    double sample = 0.0;
    float currentFrequency;
    lock.lock();
    for (std::map<int,std::pair<MidiMessage,Oscillator>>::iterator itor=currentNotes.begin();
         itor!=currentNotes.end(); ++itor)
    {
        currentFrequency = MidiMessage::getMidiNoteInHertz(itor->first, frequencySlider.getValue());
        sample += itor->second.second.oscillate(sampleRate, currentFrequency);
    }
    lock.unlock();
    return applyPan(panSlider.getValue(), sample);
}

void Synth::addNote (MidiMessage message)
{
    
    Oscillator osc (frequencySlider.getValue(),
                    dBToVolume (levelSlider.getValue()),
                    static_cast<Oscillator::WaveType> (floorf(waveSlider.getValue())));
    std::pair<MidiMessage, Oscillator> val (message, osc);
    int key = message.getNoteNumber();
    currentNotes.insert (std::pair<int, std::pair<MidiMessage, Oscillator>> (key, val));
}

void Synth::removeNote (MidiMessage message)
{
    lock.lock();
    currentNotes.erase (message.getNoteNumber());
    lock.unlock();
}

void Synth::resized()
{
    int knobHeight = 60;
    int knobWidth = 60;
    int vBorder = 25;
    int border = 5;
    frequencySlider.setBounds (border, vBorder, knobWidth, knobHeight);
    levelSlider.setBounds (border * 2 + knobWidth, vBorder, knobWidth, knobHeight);
    waveSlider.setBounds(border * 3 + knobWidth * 2, vBorder, knobWidth, knobHeight);
    panSlider.setBounds (border, vBorder * 4 + knobHeight * 3, knobWidth, knobHeight);
}

void Synth::sliderValueChanged(Slider* slider)
{
}

void Synth::updateSettings(Synth::Settings newSettings)
{
    settings = newSettings;
    updateKnobs();
}

void Synth::updateSettings(float A4Frequency, Oscillator::WaveType wave, float level, float pan)
{
    settings.A4Frequency = A4Frequency;
    settings.wave = wave;
    settings.level = dBToVolume (level);
    settings.pan = pan;
    updateKnobs();
}

void Synth::updateKnobs()
{
    frequencySlider.setValue(settings.A4Frequency);
    waveSlider.setValue(static_cast<float>(settings.wave));
    levelSlider.setValue(settings.level);
    panSlider.setValue(settings.pan);
}

Synth::Settings* Synth::getSettings()
{
    return &settings;
}
