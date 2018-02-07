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

    addAndMakeVisible (attackSliderX);
    attackSliderX.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSliderX.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (attackSliderY);
    attackSliderY.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSliderY.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (decaySliderX);
    decaySliderX.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySliderX.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (decaySliderY);
    decaySliderY.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySliderY.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (sustainSliderX);
    sustainSliderX.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSliderX.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (sustainSliderY);
    sustainSliderY.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSliderY.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    addAndMakeVisible (releaseSliderX);
    releaseSliderX.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseSliderX.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    updateADSRKnobs();

    attackSliderX.addListener(this);

    addAndMakeVisible(attackLabelX);
    attackLabelX.setText("Duration", dontSendNotification);
    attackLabelX.setFont(labelFont);
    attackLabelX.setJustificationType(labelJustification);
    attackLabelX.attachToComponent(&attackSliderX, false);

    attackSliderY.addListener(this);

    addAndMakeVisible(attackLabelY);
    attackLabelY.setText("Attack", dontSendNotification);
    attackLabelY.setFont(labelFont);
    attackLabelY.setJustificationType(labelJustification);
    attackLabelY.attachToComponent(&attackSliderY, false);

    decaySliderX.addListener(this);

    addAndMakeVisible(decayLabelX);
    decayLabelX.setText("Duration", dontSendNotification);
    decayLabelX.setFont(labelFont);
    decayLabelX.setJustificationType(labelJustification);
    decayLabelX.attachToComponent(&decaySliderX, false);

    decaySliderY.addListener(this);

    addAndMakeVisible(decayLabelY);
    decayLabelY.setText("Decay", dontSendNotification);
    decayLabelY.setFont(labelFont);
    decayLabelY.setJustificationType(labelJustification);
    decayLabelY.attachToComponent(&decaySliderY, false);

    sustainSliderX.addListener(this);

    addAndMakeVisible(sustainLabelX);
    sustainLabelX.setText("Duration", dontSendNotification);
    sustainLabelX.setFont(labelFont);
    sustainLabelX.setJustificationType(labelJustification);
    sustainLabelX.attachToComponent(&sustainSliderX, false);

    sustainSliderY.addListener(this);

    addAndMakeVisible(sustainLabelY);
    sustainLabelY.setText("Sustain", dontSendNotification);
    sustainLabelY.setFont(labelFont);
    sustainLabelY.setJustificationType(labelJustification);
    sustainLabelY.attachToComponent(&sustainSliderY, false);

    releaseSliderX.addListener(this);

    addAndMakeVisible(releaseLabelX);
    releaseLabelX.setText("Release", dontSendNotification);
    releaseLabelX.setFont(labelFont);
    releaseLabelX.setJustificationType(labelJustification);
    releaseLabelX.attachToComponent(&releaseSliderX, false);

    updateSettings(frequencySlider.getValue(),
                   static_cast<Oscillator::WaveType> (waveButtons.getValue()),
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
                    static_cast<Oscillator::WaveType> (waveButtons.getValue()),
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
    const int envelopeKnobs = 7;
    const int betweenMargin = knobWidth / 2;
    const int sideMargin = (graphWidth - (envelopeKnobs * (knobWidth + border)))
                   / 2 - (betweenMargin * (envelopeKnobs / 2));
    attackSliderY.setBounds  (leftSlidersWidth + sideMargin + border * 1 + knobWidth * 0
                              + betweenMargin * 0, graphHeight + vBorder, knobWidth, knobHeight);
    attackSliderX.setBounds  (leftSlidersWidth + sideMargin + border * 2 + knobWidth * 1
                              + betweenMargin * 0, graphHeight + vBorder, knobWidth, knobHeight);
    decaySliderY.setBounds   (leftSlidersWidth + sideMargin + border * 3 + knobWidth * 2
                              + betweenMargin * 1, graphHeight + vBorder, knobWidth, knobHeight);
    decaySliderX.setBounds   (leftSlidersWidth + sideMargin + border * 4 + knobWidth * 3
                              + betweenMargin * 1, graphHeight + vBorder, knobWidth, knobHeight);
    sustainSliderY.setBounds (leftSlidersWidth + sideMargin + border * 5 + knobWidth * 4
                              + betweenMargin * 2, graphHeight + vBorder, knobWidth, knobHeight);
    sustainSliderX.setBounds (leftSlidersWidth + sideMargin + border * 6 + knobWidth * 5
                              + betweenMargin * 2, graphHeight + vBorder, knobWidth, knobHeight);
    releaseSliderX.setBounds (leftSlidersWidth + sideMargin + border * 7 + knobWidth * 6
                              + betweenMargin * 3, graphHeight + vBorder, knobWidth, knobHeight);
}

void Synth::sliderValueChanged(Slider* slider)
{
    bool ADSRchanged = false;
    if (slider == &attackSliderX) {
        envelopeADSR.setAttackX (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &attackSliderY) {
        envelopeADSR.setAttackY (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &decaySliderX) {
        envelopeADSR.setDecayX (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &decaySliderY) {
        envelopeADSR.setDecayY (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &sustainSliderX) {
        envelopeADSR.setSustainX (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &sustainSliderY) {
        envelopeADSR.setSustainY (slider->getValue());
        ADSRchanged = true;
    } else if (slider == &releaseSliderX) {
        envelopeADSR.setReleaseX (slider->getValue());
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
    envelopeADSR.setAttack (attack, &attackSliderX, &attackSliderY);
    envelopeADSR.setDecay (decay, &decaySliderX, &decaySliderY);
    envelopeADSR.setSustain (sustain, &sustainSliderX, &sustainSliderY);
    envelopeADSR.setReleaseX (release, &releaseSliderX);
    updateADSRKnobs();
}

void Synth::updateADSRKnobs()
{
    attackSliderX.setRange  (envelopeADSR.getStartPoint(),
                             envelopeADSR.getDecay().getX() - 1.0,
                             dontSendNotification);
    attackSliderX.setValue  (envelopeADSR.getAttack().getX(),
                             dontSendNotification);
    attackSliderY.setRange  (0.0, envelopeADSR.getMaxdB(),
                             dontSendNotification);
    attackSliderY.setValue  (envelopeADSR.getAttack().getY(),
                             dontSendNotification);
    decaySliderX.setRange   (envelopeADSR.getAttack().getX() + 1.0,
                             envelopeADSR.getSustain().getX() - 1.0,
                             dontSendNotification);
    decaySliderX.setValue   (envelopeADSR.getDecay().getX(),
                             dontSendNotification);
    decaySliderY.setRange   (0.0, envelopeADSR.getMaxdB(),
                             dontSendNotification);
    decaySliderY.setValue   (envelopeADSR.getDecay().getY(),
                             dontSendNotification);
    sustainSliderX.setRange (envelopeADSR.getDecay().getX() + 1.0,
                             envelopeADSR.getRelease().getX() - 1.0,
                             dontSendNotification);
    sustainSliderX.setValue (envelopeADSR.getSustain().getX(),
                             dontSendNotification);
    sustainSliderY.setRange (0.0, envelopeADSR.getMaxdB(),
                             dontSendNotification);
    sustainSliderY.setValue (envelopeADSR.getSustain().getY(),
                             dontSendNotification);
    releaseSliderX.setRange (envelopeADSR.getSustain().getX() + 1.0,
                             envelopeADSR.getMaxMS(),
                             dontSendNotification);
    releaseSliderX.setValue (envelopeADSR.getRelease().getX(),
                             dontSendNotification);
    attackSliderX.repaint();
    attackSliderY.repaint();
    decaySliderX.repaint();
    decaySliderY.repaint();
    sustainSliderX.repaint();
    sustainSliderY.repaint();
    releaseSliderX.repaint();
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

Synth::Settings* Synth::getSettings()
{
    return &settings;
}
