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
    startPointMS = 0;
    setAttack (1000, 50);
    setDecay (1500, 30);
    setSustain (6000, 30);
    setReleaseX (6500); // this sets endPointMS
}

ADSR::~ADSR()
{
}

float ADSR::getStartPoint()
{
    return startPointMS;
}
int ADSR::getMaxMS()
{
    return maxMS;
}
int ADSR::getMaxdB()
{
    return maxdB;
}

Point<float> ADSR::getAttack()
{
    return attack;
}
Point<float> ADSR::getDecay()
{
    //auto adjustedPoint = Point<float> (decay.getX() + attack.getX(), decay.getY());
    return decay;
}
Point<float> ADSR::getSustain()
{
    //auto adjustedPoint = Point<float> (sustain.getX() + decay.getX() + attack.getX(),
    //                                   decay.getY());
    return sustain;
}
Point<float> ADSR::getRelease()
{
    //return Point<float> (endPointMS + sustain.getX() + decay.getX() + attack.getX(), 0.0);
    return Point<float> (endPointMS, 0.0);
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
    endPoint = getRelease().getX() * Xmult;
    auto attackPoint = Point<float> (getAttack().getX() * Xmult,
                                     static_cast<float> (area.getHeight()) - attack.getY() * Ymult);
    auto decayPoint = Point<float> (getDecay().getX() * Xmult,
                                    static_cast<float> (area.getHeight()) - decay.getY() * Ymult);
    auto sustainPoint = Point<float> (getSustain().getX() * Xmult,
                                      static_cast<float> (area.getHeight()) - sustain.getY() * Ymult);
    addPoint (attackPoint);
    addPoint (decayPoint);
    addPoint (sustainPoint);
    repaint();
}

void ADSR::setAttack (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - attack.getX();
    attack = point;
    //decay.setX (decay.getX() + diff);
    //sustain.setX (sustain.getX() + diff);
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getAttack().getX());
    if (sliderY != nullptr) sliderY->setValue (getAttack().getY());
    redraw();
}
void ADSR::setAttack (int x, int y, Slider* sliderX, Slider* sliderY)
{
    setAttack (Point<float> (static_cast<float> (x), static_cast<float> (y)), sliderX, sliderY);
}
void ADSR::setAttackX (int x, Slider* sliderX)
{
    setAttack (Point<float> (static_cast<float> (x), attack.getY()), sliderX, nullptr);
}
void ADSR::setAttackY (int y, Slider* sliderY)
{
    setAttack (Point<float> (attack.getX(), static_cast<float> (y)), nullptr, sliderY);
}

void ADSR::setDecay (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - decay.getX();
    //auto adjustedPoint = Point<float> (point.getX() - attack.getX(), point.getY());
    decay = point;
    //sustain.setX (sustain.getX() + diff);
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getDecay().getX());
    if (sliderY != nullptr) sliderY->setValue (getDecay().getY());
    redraw();
}
void ADSR::setDecay (int x, int y, Slider* sliderX, Slider* sliderY)
{
    setDecay (Point<float> (static_cast<float> (x), static_cast<float> (y)), sliderX, sliderY);
}
void ADSR::setDecayX (int x, Slider* sliderX)
{
    setDecay (Point<float> (static_cast<float> (x), decay.getY()), sliderX, nullptr);
}
void ADSR::setDecayY (int y, Slider* sliderY)
{
    setDecay (Point<float> (decay.getX(), static_cast<float> (y)), nullptr, sliderY);
}

void ADSR::setSustain (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - sustain.getX();
    //auto adjustedPoint = Point<float> (point.getX() - attack.getX() - decay.getX(),
    //                                   point.getY());
    sustain = point;
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getSustain().getX());
    if (sliderY != nullptr) sliderY->setValue (getSustain().getY());
    redraw();
}
void ADSR::setSustain (int x, int y, Slider* sliderX, Slider* sliderY)
{
    setSustain (Point<float> (static_cast<float> (x), static_cast<float> (y)), sliderX, sliderY);
}
void ADSR::setSustainX (int x, Slider* sliderX)
{
    setSustain (Point<float> (static_cast<float> (x), sustain.getY()), sliderX, nullptr);
}
void ADSR::setSustainY (int y, Slider* sliderY)
{
    setSustain (Point<float> (sustain.getX(), static_cast<float> (y)), nullptr, sliderY);
}

void ADSR::setReleaseX (float x, Slider* sliderX)
{
    //endPointMS = x - sustain.getX() - decay.getX() - attack.getX();
    endPointMS = x;
    if (sliderX != nullptr) sliderX->setValue (getRelease().getX());
    redraw();
}
void ADSR::setReleaseX (Point<float> point, Slider* sliderX)
{
    setReleaseX (point.getX(), sliderX);
}
/* this one causes ambiguous overload
void ADSR::setReleaseX (int x, Slider* sliderX)
{
    setReleaseX (static_cast<float> (x), sliderX);
}
*/

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
                   static_cast<Oscillator::WaveType> (static_cast<int>(floorf(waveSlider.getValue()))),
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
                    static_cast<Oscillator::WaveType> (static_cast<int>(floorf(waveSlider.getValue()))));
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
    int topMargin = 4;
    int topMarginKnob = 24;
    int knobHeight = 60;
    int knobWidth = 60;
    int vBorder = 50;
    int border = 5;
    int graphHeight = (vBorder / 2 + knobHeight) * 2 - topMarginKnob - knobHeight;
    int graphWidth = area.getWidth() - (border * 4 + knobWidth * 2);
    frequencySlider.setBounds (border, topMarginKnob, knobWidth, knobHeight);
    levelSlider.setBounds (border * 2 + knobWidth, topMarginKnob, knobWidth, knobHeight);
    panSlider.setBounds(border, vBorder + knobHeight + topMarginKnob, knobWidth, knobHeight);
    waveSlider.setBounds (border * 2 + knobWidth, vBorder + knobHeight + topMarginKnob, knobWidth, knobHeight);
    int leftSlidersWidth = border * 3 + knobWidth * 2;
    envelopeMenu.setBounds (leftSlidersWidth, topMargin, 250, 16);
    envelopeADSR.setBounds (leftSlidersWidth, topMarginKnob, graphWidth, graphHeight);
    // center justification
    int envelopeKnobs = 7;
    int betweenMargin = knobWidth / 2;
    int sideMargin = (graphWidth - (envelopeKnobs * (knobWidth + border)))
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
    waveSlider.setValue(static_cast<float>(settings.wave));
    levelSlider.setValue(settings.level);
    panSlider.setValue(settings.pan);
}

void Synth::updateTree(ValueTree & t)
{
	addValue(t, frequencySlider.getValue(), "a4freq");
	addValue(t, waveSlider.getValue(), "wave");
	addValue(t, levelSlider.getValue(), "level");
	addValue(t, panSlider.getValue(), "pan");
	//t.setProperty(id, var(frequencySlider.getValue()), nullptr);
	//t.setProperty(id + "_1", var(waveSlider.getValue()), nullptr);
	//t.setProperty(id + "_2", var(levelSlider.getValue()), nullptr);
	//t.setProperty(id + "_3", var(panSlider.getValue()), nullptr);
}

void Synth::updateValues(ValueTree & t)
{
	setValue(t, settings.A4Frequency, "a4freq");
	setValue(t, settings.wave, "wave");
	setValue(t, settings.level, "level");
	setValue(t, settings.pan, "pan");
	updateKnobs();
}

Synth::Settings* Synth::getSettings()
{
    return &settings;
}
