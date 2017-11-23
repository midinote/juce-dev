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

ADSR::ADSR (Colour backgroundColour, Colour colour) : Graph(backgroundColour, colour)
{
    // defaults
    maxMS = 10000;
    maxdB = 60;
    startPointMS = 50;
    setAttack (100, 50);
    setDecay (500, 30);
    setSustain (5000, 30);
    setReleaseX (5500); // this sets endPointMS
}

ADSR::~ADSR()
{
}

Point<float> ADSR::getAttack()
{
    return attack;
}

Point<float> ADSR::getDecay()
{
    return decay;
}

Point<float> ADSR::getSustain()
{
    return sustain;
}

Point<float> ADSR::getRelease()
{
    return Point<float> (endPoint, 0.0);
}

void ADSR::redraw()
{
    clear();
    // use multiplication to make the bounds of the graph represent the
    // bounds of the miliseconds and decibels
    Rectangle<int> area = getLocalBounds();
    float Xmult = static_cast<float> (area.getWidth()) / static_cast<float> (maxMS);
    float Ymult = static_cast<float> (area.getHeight()) / static_cast<float> (maxdB);
    startPoint = startPointMS * Xmult;
    endPoint = endPointMS * Xmult;
    auto attackPoint = Point<float> (attack.getX() * Xmult,
                                     static_cast<float> (area.getHeight()) - attack.getY() * Ymult);
    auto decayPoint = Point<float> (decay.getX() * Xmult,
                                    static_cast<float> (area.getHeight()) - decay.getY() * Ymult);
    auto sustainPoint = Point<float> (sustain.getX() * Xmult,
                                      static_cast<float> (area.getHeight()) - sustain.getY() * Ymult);
    addPoint (attackPoint);
    addPoint (decayPoint);
    addPoint (sustainPoint);
    repaint();
}

void ADSR::setAttack (Point<float> point)
{
    attack = point;
    redraw();
}

void ADSR::setAttack (int x, int y)
{
    setAttack (Point<float> (static_cast<float> (x), static_cast<float> (y)));
}

void ADSR::setDecay (Point<float> point)
{
    decay = point;
    redraw();
}

void ADSR::setDecay (int x, int y)
{
    setDecay (Point<float> (static_cast<float> (x), static_cast<float> (y)));
}

void ADSR::setSustain (Point<float> point)
{
    sustain = point;
    redraw();
}

void ADSR::setSustain (int x, int y)
{
    setSustain (Point<float> (static_cast<float> (x), static_cast<float> (y)));
}

void ADSR::setReleaseX (Point<float> point)
{
    endPointMS = point.getX();
    redraw();
}

void ADSR::setReleaseX (int x)
{
    setReleaseX (Point<float> (static_cast<float> (x), 0.0));
}

void ADSR::resized()
{
    Graph::resized();
    redraw();
}


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
    frequencyLabel.setText("A4 Freq", dontSendNotification);
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

    addAndMakeVisible (envelopeMenu);
    envelopeMenu.addItem("Envelope", 1);
    envelopeMenu.addItem("Filter", 2);
    envelopeMenu.addItem("LFO", 3);
    envelopeMenu.setSelectedId(1, dontSendNotification);
    addAndMakeVisible (envelopeADSR);
    envelopeADSR.drawOn();

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
    g.fillAll (Colour (84, 85, 84));
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
    Rectangle<int> area = getBounds();
    int knobHeight = 60;
    int knobWidth = 60;
    int vBorder = 25;
    int border = 5;
    frequencySlider.setBounds (border, vBorder, knobWidth, knobHeight);
    levelSlider.setBounds (border * 2 + knobWidth, vBorder, knobWidth, knobHeight);
    panSlider.setBounds(border, vBorder * 2 + knobHeight, knobWidth, knobHeight);
    waveSlider.setBounds (border * 2 + knobWidth, vBorder * 2 + knobHeight, knobWidth, knobHeight);
    envelopeMenu.setBounds (border * 3 + knobWidth * 2, 4, 250, 16);
    envelopeADSR.setBounds (border * 3 + knobWidth * 2, 24,
                            area.getWidth() - (border * 4 + knobWidth * 2),
                            (vBorder + knobHeight) * 2 - 24);
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
