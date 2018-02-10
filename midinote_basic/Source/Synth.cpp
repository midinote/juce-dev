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
    waveButtons (Oscillator::sine, 5, Font (12), Justification::centredLeft,
                 { "Sine", "Square", "Triangle", "Sawtooth", "Noise" })
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

    addAndMakeVisible (waveButtons);
    waveButtons.addListenerToButtons (this);

    addAndMakeVisible(waveLabel);
    waveLabel.setText("Wavetype", dontSendNotification);
    waveLabel.setFont(labelFont);
    waveLabel.setJustificationType(labelJustification);
    waveLabel.attachToComponent(&waveButtons, false);

    addAndMakeVisible (envelopeMenu);
    envelopeMenu.addItem("Envelope", 1);
    envelopeMenu.addItem("Filter", 2);
    envelopeMenu.addItem("LFO", 3);
    envelopeMenu.setSelectedId(1, dontSendNotification);
    addAndMakeVisible (envelopeADSR);
    envelopeADSR.drawOn();

    addAndMakeVisible (attackSlider);
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (decaySlider);
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (sustainSlider);
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (releaseSlider);
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    updateADSRKnobs();

    attackSlider.addListener(this);

    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", dontSendNotification);
    attackLabel.setFont(labelFont);
    attackLabel.setJustificationType(labelJustification);
    attackLabel.attachToComponent(&attackSlider, false);

    decaySlider.addListener(this);

    addAndMakeVisible(decayLabel);
    decayLabel.setText("Decay", dontSendNotification);
    decayLabel.setFont(labelFont);
    decayLabel.setJustificationType(labelJustification);
    decayLabel.attachToComponent(&decaySlider, false);

    sustainSlider.addListener(this);

    addAndMakeVisible(sustainLabel);
    sustainLabel.setText("Sustain", dontSendNotification);
    sustainLabel.setFont(labelFont);
    sustainLabel.setJustificationType(labelJustification);
    sustainLabel.attachToComponent(&sustainSlider, false);

    releaseSlider.addListener(this);

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
    const int vBorder = 50;
    const int border = 5;
    const int graphHeight = (vBorder / 2 + knobHeight) * 2 - topMarginKnob - knobHeight;
    const int graphWidth = area.getWidth() - (border * 4 + knobWidth * 2);
    frequencySlider.setBounds (border, topMarginKnob, knobWidth, knobHeight);
    levelSlider.setBounds (border * 2 + knobWidth, topMarginKnob, knobWidth, knobHeight);
    panSlider.setBounds (border * 3 + knobWidth * 2, topMarginKnob, knobWidth, knobHeight);
    const int leftSlidersHeight = vBorder + border + knobHeight;
    waveButtons.setBounds (border, leftSlidersHeight, knobWidth * 3 + border * 2,
                           area.getHeight() - leftSlidersHeight - border);
    const int leftSlidersWidth = border * 4 + knobWidth * 3;
    envelopeMenu.setBounds (leftSlidersWidth, topMargin, 250, 16);
    envelopeADSR.setBounds (leftSlidersWidth, topMarginKnob, graphWidth, graphHeight);
    // center justification of ADSR knobs
    const int envelopeKnobs = 4;
    const int betweenMargin = knobWidth / 2;
    const int sideMargin = (graphWidth - (envelopeKnobs * (knobWidth + border)))
                   / 2 - (betweenMargin * (envelopeKnobs / 2));
    attackSlider.setBounds  (leftSlidersWidth + sideMargin + border * 1 + knobWidth * 0
                             + betweenMargin * 1, graphHeight + vBorder,
                             knobWidth, knobHeight);
    decaySlider.setBounds  (leftSlidersWidth + sideMargin + border * 2 + knobWidth * 1
                            + betweenMargin * 1, graphHeight + vBorder,
                            knobWidth, knobHeight);
    sustainSlider.setBounds   (leftSlidersWidth + sideMargin + border * 3 + knobWidth * 2
                               + betweenMargin * 1, graphHeight + vBorder,
                               knobWidth, knobHeight);
    releaseSlider.setBounds   (leftSlidersWidth + sideMargin + border * 4 + knobWidth * 3
                               + betweenMargin * 1, graphHeight + vBorder,
                               knobWidth, knobHeight);
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
                            envelopeADSR.getDecay() - 1.0,
                            dontSendNotification);
    attackSlider.setValue  (envelopeADSR.getAttack(), dontSendNotification);
    decaySlider.setRange   (envelopeADSR.getAttack() + 1.0,
                            envelopeADSR.getRelease() - 1.0,
                            dontSendNotification);
    decaySlider.setValue   (envelopeADSR.getDecay(), dontSendNotification);
    sustainSlider.setRange (0, envelopeADSR.getMaxdB(),
                            dontSendNotification);
    sustainSlider.setValue (envelopeADSR.getSustain(), dontSendNotification);
    releaseSlider.setRange (envelopeADSR.getDecay() + 1.0,
                            envelopeADSR.getMaxMS(),
                            dontSendNotification);
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
