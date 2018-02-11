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
    labelJustification(Justification::centredTop),
    filter (Font (12), Justification::centredLeft),
    waveButtons (Oscillator::sine, Oscillator::WAVETYPE_SIZE, Font (12),
                 Justification::centredLeft,
                 { "Sine", "Square", "Triangle", "Sawtooth", "Noise" })
{
    addAndMakeVisible (frequencySlider);
    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    frequencySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    frequencySlider.setRange (55.0, 14080.0, 1);
    frequencySlider.setSkewFactorFromMidPoint (440.0);
    frequencySlider.setValue(440.0);
    frequencySlider.addListener (this);

    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("A4 Freq", dontSendNotification);
    frequencyLabel.setFont(labelFont);
    frequencyLabel.setJustificationType(labelJustification);
    frequencyLabel.attachToComponent(&frequencySlider, false);

    addAndMakeVisible(levelSlider);
    levelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    levelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    levelSlider.setRange (-100, 6.0, 0.1);
    levelSlider.setSkewFactorFromMidPoint(-6.0);
    levelSlider.setValue(0.0);
    levelSlider.addListener (this);

    addAndMakeVisible(levelLabel);
    levelLabel.setText("Level", dontSendNotification);
    levelLabel.setFont(labelFont);
    levelLabel.setJustificationType(labelJustification);
    levelLabel.attachToComponent(&levelSlider, false);

    addAndMakeVisible(panSlider);
    panSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    panSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    panSlider.setRange(-0.5, 0.5, 0.01);
    panSlider.setValue(0.0);
    panSlider.addListener (this);

    addAndMakeVisible(panLabel);
    panLabel.setText("Pan", dontSendNotification);
    panLabel.setFont(labelFont);
    panLabel.setJustificationType(labelJustification);
    panLabel.attachToComponent(&panSlider, false);

    addAndMakeVisible (waveButtons);
    waveButtons.addListenerToButtons (this);

    addAndMakeVisible (waveLabel);
    waveLabel.setText ("Wavetype", dontSendNotification);
    waveLabel.setFont (labelFont);
    waveLabel.setJustificationType (labelJustification);
    waveLabel.attachToComponent (&waveButtons, false);

    addAndMakeVisible (filter);
    filter.addListenerToButtons (this);

    addAndMakeVisible (filterModeLabel);
    filterModeLabel.setText ("Filter Mode", dontSendNotification);
    filterModeLabel.setFont (labelFont);
    filterModeLabel.setJustificationType (labelJustification);
    filterModeLabel.attachToComponent (&filter, false);

    addAndMakeVisible (cutoffSlider);
    cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    cutoffSlider.setRange(10, 20000, 1);
    cutoffSlider.setValue(filter.cutoff);
    cutoffSlider.addListener (this);

    addAndMakeVisible(cutoffLabel);
    cutoffLabel.setText("Cutoff", dontSendNotification);
    cutoffLabel.setFont(labelFont);
    cutoffLabel.setJustificationType(labelJustification);
    cutoffLabel.attachToComponent(&cutoffSlider, false);

    addAndMakeVisible (resonanceSlider);
    resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    resonanceSlider.setRange(-3.0, 30.1, 0.1);
    resonanceSlider.setValue(filter.resonance);
    resonanceSlider.addListener (this);

    addAndMakeVisible(resonanceLabel);
    resonanceLabel.setText("Resonance", dontSendNotification);
    resonanceLabel.setFont(labelFont);
    resonanceLabel.setJustificationType(labelJustification);
    resonanceLabel.attachToComponent(&resonanceSlider, false);

    addAndMakeVisible (envelopeMenu);
    envelopeMenu.addItem ("Envelope", 1);
    envelopeMenu.addItem ("Filter", 2);
    envelopeMenu.addItem ("LFO", 3);
    envelopeMenu.setSelectedId (1, dontSendNotification);
    addAndMakeVisible (envelopeADSR);
    envelopeADSR.drawOn();

    addAndMakeVisible (attackSlider);
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    addAndMakeVisible (decaySlider);
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    addAndMakeVisible (sustainSlider);
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    addAndMakeVisible (releaseSlider);
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    updateADSRKnobs();

    attackSlider.addListener (this);

    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", dontSendNotification);
    attackLabel.setFont(labelFont);
    attackLabel.setJustificationType(labelJustification);
    attackLabel.attachToComponent(&attackSlider, false);

    decaySlider.addListener (this);

    addAndMakeVisible(decayLabel);
    decayLabel.setText("Decay", dontSendNotification);
    decayLabel.setFont(labelFont);
    decayLabel.setJustificationType(labelJustification);
    decayLabel.attachToComponent(&decaySlider, false);

    sustainSlider.addListener (this);

    addAndMakeVisible(sustainLabel);
    sustainLabel.setText("Sustain", dontSendNotification);
    sustainLabel.setFont(labelFont);
    sustainLabel.setJustificationType(labelJustification);
    sustainLabel.attachToComponent(&sustainSlider, false);

    releaseSlider.addListener (this);

    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", dontSendNotification);
    releaseLabel.setFont(labelFont);
    releaseLabel.setJustificationType(labelJustification);
    releaseLabel.attachToComponent(&releaseSlider, false);

    updateSettings(frequencySlider.getValue(),
                   static_cast<Oscillator::WaveType> (static_cast<int>(waveButtons.getValue())),
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
                    static_cast<Oscillator::WaveType> (static_cast<int>(waveButtons.getValue())),
                    &envelopeADSR);
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
    const int topMargin = 4;
    const int topMarginKnob = 24;
    const int knobHeight = 60;
    const int knobWidth = 60;
    const int knobWidthBigNumbers = knobWidth + 20;
    const int vBorder = 50;
    const int border = 5;
    const int radialButtonsWidth = knobWidth * 3 + border * 2;
    const int graphHeight = (vBorder / 2 + knobHeight) * 2 - topMarginKnob - knobHeight;
    const int graphWidth = area.getWidth() - (border * 4 + knobWidth * 2);
    frequencySlider.setBounds (border, topMarginKnob, knobWidth, knobHeight);
    levelSlider.setBounds (border * 2 + knobWidth, topMarginKnob, knobWidth, knobHeight);
    panSlider.setBounds (border * 3 + knobWidth * 2, topMarginKnob, knobWidth, knobHeight);
    const int leftSlidersHeight = vBorder + border + knobHeight;
    waveButtons.setBounds (border, leftSlidersHeight, radialButtonsWidth,
                           area.getHeight() - leftSlidersHeight - border);
    const int leftSlidersWidth = border * 4 + knobWidth * 3;
    envelopeMenu.setBounds (leftSlidersWidth, topMargin, 250, 16);
    envelopeADSR.setBounds (leftSlidersWidth, topMarginKnob, graphWidth, graphHeight);
    // center justification of Filter & ADSR knobs & buttons
    const int envelopeKnobs = 6;
    const int betweenMargin = knobWidthBigNumbers / 2;
    const int sideMargin = (graphWidth - (radialButtonsWidth + envelopeKnobs * (knobWidthBigNumbers + border))) / 2 - (betweenMargin * 2);
    filter.setBounds (leftSlidersWidth + sideMargin, graphHeight + vBorder + border,
                      radialButtonsWidth, knobHeight);
    cutoffSlider.setBounds    (leftSlidersWidth + radialButtonsWidth + sideMargin + border * 1 + knobWidthBigNumbers * 0 + betweenMargin * 1, graphHeight + vBorder,
                               knobWidthBigNumbers, knobHeight);
    resonanceSlider.setBounds (leftSlidersWidth + radialButtonsWidth + sideMargin + border * 2 + knobWidthBigNumbers * 1 + betweenMargin * 1, graphHeight + vBorder,
                               knobWidthBigNumbers, knobHeight);
    attackSlider.setBounds    (leftSlidersWidth + radialButtonsWidth + sideMargin + border * 3 + knobWidthBigNumbers * 2 + betweenMargin * 2, graphHeight + vBorder,
                               knobWidthBigNumbers, knobHeight);
    decaySlider.setBounds     (leftSlidersWidth + radialButtonsWidth + sideMargin + border * 4 + knobWidthBigNumbers * 3 + betweenMargin * 2, graphHeight + vBorder,
                               knobWidthBigNumbers, knobHeight);
    sustainSlider.setBounds   (leftSlidersWidth + radialButtonsWidth + sideMargin + border * 5 + knobWidthBigNumbers * 4 + betweenMargin * 2, graphHeight + vBorder,
                               knobWidthBigNumbers, knobHeight);
    releaseSlider.setBounds   (leftSlidersWidth + radialButtonsWidth + sideMargin + border * 6 + knobWidthBigNumbers * 5 + betweenMargin * 2, graphHeight + vBorder,
                               knobWidthBigNumbers, knobHeight);
}

void Synth::sliderValueChanged(Slider* slider)
{
    bool ADSRchanged = false;
    if (slider == &attackSlider) {
        envelopeADSR.setAttack (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &decaySlider) {
        envelopeADSR.setDecay (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &sustainSlider) {
        envelopeADSR.setSustain (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &releaseSlider) {
        envelopeADSR.setRelease (slider->getValue());
        ADSRchanged = true;
    }
    if (ADSRchanged) updateADSRKnobs();
}

void Synth::buttonClicked (Button* button)
{
    if (waveButtons.contains (button))
        settings.wave = static_cast<Oscillator::WaveType> (waveButtons.clicked (button));
    else if (filter.contains (button)) {
        filter.clicked (button);
        // set the synth accordingly? might not be necessary depending on how that gets implemented
    }
}

void Synth::updateADSR (Point<float> attack, Point<float> decay, Point<float> sustain, float release)
{
    envelopeADSR.setAttack (attack, &attackSlider);
    envelopeADSR.setDecay (decay, &decaySlider);
    envelopeADSR.setSustain (sustain, &sustainSlider);
    envelopeADSR.setRelease (release, &releaseSlider);
    updateADSRKnobs();
}
void Synth::updateADSR (float attack, float decay, float sustain, float release)
{
    envelopeADSR.setAttack (attack, &attackSlider);
    envelopeADSR.setDecay (decay, &decaySlider);
    envelopeADSR.setSustain (sustain, &sustainSlider);
    envelopeADSR.setRelease (release, &releaseSlider);
    updateADSRKnobs();
}
void Synth::updateADSRKnobs()
{
    attackSlider.setRange  (envelopeADSR.getStartPoint(),
                            envelopeADSR.getMaxMS() - envelopeADSR.getDecay() - envelopeADSR.getRelease() - 1.0, 0.1);
    attackSlider.setValue  (envelopeADSR.getAttack(), dontSendNotification);
    decaySlider.setRange   (0,
                            envelopeADSR.getMaxMS() - envelopeADSR.getAttack() - envelopeADSR.getRelease() - 1.0, 0.1);
    decaySlider.setValue   (envelopeADSR.getDecay(), dontSendNotification);
    sustainSlider.setRange (0, envelopeADSR.getMaxdB(), 0.1);
    sustainSlider.setValue (envelopeADSR.getSustain(), dontSendNotification);
    releaseSlider.setRange (0,
                            envelopeADSR.getMaxMS() - envelopeADSR.getAttack() - envelopeADSR.getDecay() - 1.0, 0.1);
    releaseSlider.setValue (envelopeADSR.getRelease(), dontSendNotification);
    attackSlider.repaint();
    decaySlider.repaint();
    sustainSlider.repaint();
    releaseSlider.repaint();
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
    waveButtons.setValue(settings.wave);
    levelSlider.setValue(settings.level);
    panSlider.setValue(settings.pan);
}

void Synth::updateTree(ValueTree & t)
{
	addValue(t, frequencySlider.getValue(), "a4freq");
	addValue(t, waveButtons.getValue(), "wave");
	addValue(t, levelSlider.getValue(), "level");
	addValue(t, panSlider.getValue(), "pan");
	//t.setProperty(id, var(frequencySlider.getValue()), nullptr);
	//t.setProperty(id + "_1", var(waveSlider.getValue()), nullptr);
	//t.setProperty(id + "_2", var(levelSlider.getValue()), nullptr);
	//t.setProperty(id + "_3", var(panSlider.getValue()), nullptr);
    addValue(t, envelopeADSR.getAttack(), "attack");
    addValue(t, envelopeADSR.getDecay(), "decay");
    addValue(t, envelopeADSR.getSustain(), "sustain");
    addValue(t, envelopeADSR.getRelease(), "release");
}

void Synth::updateValues(ValueTree & t)
{
	setValue(t, settings.A4Frequency, "a4freq");
	setValue(t, settings.wave, "wave");
	setValue(t, settings.level, "level");
	setValue(t, settings.pan, "pan");
	updateKnobs();
    float attack = -1.0, decay = -1.0, sustain = -1.0, release = -1.0;
    setValue(t, attack, "attack");
    setValue(t, decay, "decay");
    setValue(t, sustain, "sustain");
    setValue(t, release, "release");
    bool success = true;
    if (attack < 0.0) std::cout << "could not receive attack" << std::endl;
    if (decay < 0.0) std::cout << "could not receive decay" << std::endl;
    if (sustain < 0.0) std::cout << "could not receive sustain" << std::endl;
    if (release < 0.0) std::cout << "could not receive release" << std::endl;
    if (success) updateADSR (attack, decay, sustain, release);
}

Synth::Settings* Synth::getSettings()
{
    return &settings;
}
