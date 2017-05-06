/*
  ==============================================================================

    Synth.cpp
    Created: 7 Apr 2017 7:05:20pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "MainComponent.h"
#include "Synth.h"
#include "MidiNoteFunctionLib.h"

//==============================================================================

/*
 ====================
 = SynthAudioSource =
 ====================
 */

SynthAudioSource::SynthAudioSource (MidiKeyboardState& keyState) : keyboardState (keyState)
{
    sounds = {};
    for (int i = 0; i < 64; i++) synth.addVoice (new Oscillator());
    for (int i = 0; i < 3; i++) addWaveSound(WaveSound::sine, 1.0, 0.0, i);
}

void SynthAudioSource::addWaveSound(WaveSound::Wave wave, float level, float pan, int oscNum)
{
    WaveSound* sound = new WaveSound(level, pan, wave);
    sounds.at (oscNum) = sound;
}

void SynthAudioSource::applySounds ()
{
    synth.clearSounds();
    for (std::vector<WaveSound*>::iterator itor = sounds.begin(); itor != sounds.end(); ++itor)
        synth.addSound(*itor);
}

void SynthAudioSource::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
{
    midiCollector.reset (sampleRate);

    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void SynthAudioSource::releaseResources ()
{
}

void SynthAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
    keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);
    synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
}

/*
 ========================
 = Oscillator Component =
 ========================
 */

OscillatorComponent::OscillatorComponent()
:   waveType(WaveSound::sine),
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
    
    addAndMakeVisible (fineTuningSlider);
    fineTuningSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    fineTuningSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    fineTuningSlider.setRange (-5.0, 5.0);
    fineTuningSlider.setValue(0.0);
    fineTuningSlider.addListener(this);
    
    addAndMakeVisible(fineTuningLabel);
    fineTuningLabel.setText("Fine\nTuning", dontSendNotification);
    fineTuningLabel.setFont(labelFont);
    fineTuningLabel.setJustificationType(labelJustification);
    fineTuningLabel.attachToComponent(&fineTuningSlider, false);

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

    addAndMakeVisible(squareButton);
    squareButton.setClickingTogglesState(true);
    squareButton.setRadioGroupId(0);
    
    addAndMakeVisible(sineButton);
    sineButton.setClickingTogglesState(true);
    sineButton.setRadioGroupId(0);
    sineButton.setToggleState(true, dontSendNotification);

    addAndMakeVisible(sawtoothButton);
    sawtoothButton.setClickingTogglesState(true);
    sawtoothButton.setRadioGroupId(0);

    addAndMakeVisible(triangleButton);
    triangleButton.setClickingTogglesState(true);
    triangleButton.setRadioGroupId(0);

    addAndMakeVisible(noiseButton);
    noiseButton.setClickingTogglesState(true);
    noiseButton.setRadioGroupId(0);

    addAndMakeVisible(panSlider);
    panSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    panSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    panSlider.setRange(-1.0,1.0);
    panSlider.setValue(0.0);
    panSlider.addListener(this);

    addAndMakeVisible(panLabel);
    panLabel.setText("Pan", dontSendNotification);
    panLabel.setFont(labelFont);
    panLabel.setJustificationType(labelJustification);
    panLabel.attachToComponent(&panSlider, false);

    updateSettings(frequencySlider.getValue(),
                   WaveSound::sine,
                   dBToVolume(levelSlider.getValue()),
                   panSlider.getValue());
}

OscillatorComponent::~OscillatorComponent()
{
}

void OscillatorComponent::paint (Graphics& g)
{
}

void OscillatorComponent::resized()
{
    int knobHeight = 60;
    int knobWidth = 60;
    int vBorder = 25;
    int border = 5;
    frequencySlider.setBounds (border, vBorder, knobWidth, knobHeight);
    fineTuningSlider.setBounds (border * 2 + knobWidth, vBorder, knobWidth, knobHeight);
    levelSlider.setBounds (border * 3 + knobWidth * 2, vBorder, knobWidth, knobHeight);
    panSlider.setBounds (border * 4 + knobWidth * 3, vBorder, knobWidth, knobHeight);
}

void OscillatorComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &frequencySlider) settings.A4Frequency = slider->getValue();
    else if (slider == &levelSlider) settings.level = slider->getValue();
    else if (slider == &panSlider) settings.pan = slider->getValue();
}

void OscillatorComponent::buttonClicked(Button* button)
{
    if (button == &squareButton) settings.wave = WaveSound::square;
    else if (button == &sineButton) settings.wave = WaveSound::sine;
    else if (button == &sawtoothButton) settings.wave = WaveSound::sawtooth;
    else if (button == &triangleButton) settings.wave = WaveSound::triangle;
    else if (button == &noiseButton) settings.wave = WaveSound::noise;
}

void OscillatorComponent::updateSettings(OscillatorComponent::Settings newSettings)
{
    settings = newSettings;
    updateKnobs();
}

void OscillatorComponent::updateSettings(float A4Frequency, WaveSound::Wave wave, float level, float pan)
{
    settings.A4Frequency = A4Frequency;
    settings.wave = wave;
    settings.level = dBToVolume (level);
    settings.pan = pan;
    updateKnobs();
}

void OscillatorComponent::updateKnobs()
{
    frequencySlider.setValue(settings.A4Frequency);
    fineTuningSlider.setValue(0.0);
    levelSlider.setValue(settings.level);
    panSlider.setValue(settings.pan);
}

OscillatorComponent::Settings OscillatorComponent::getSettings()
{
    return settings;
}

/*
 ==================
 = SynthComponent =
 ==================
 */

SynthComponent::SynthComponent ()
    : deviceManager (MainContentComponent::getSharedAudioDeviceManager()),
      synthAudioSource (keyboardState),
      keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(osc0);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    
    settings.osc0Settings = osc0.getSettings();
    settings.osc1Settings = osc1.getSettings();
    settings.osc2Settings = osc2.getSettings();
    audioSourcePlayer.setSource (&synthAudioSource);
    deviceManager.addAudioCallback (&audioSourcePlayer);
    deviceManager.addMidiInputCallback (String(), &(synthAudioSource.midiCollector));
}

SynthComponent::~SynthComponent ()
{
    audioSourcePlayer.setSource (nullptr);
    deviceManager.removeMidiInputCallback (String(), &(synthAudioSource.midiCollector));
    deviceManager.removeAudioCallback (&audioSourcePlayer);
}

SynthComponent::Settings* SynthComponent::getSettings()
{
    return &settings;
}

void SynthComponent::updateSettings(Settings newSettings)
{
    settings = newSettings;
}


void SynthComponent::paint (Graphics& g)
{
}

void SynthComponent::resized()
{
    int oscHeight = 70;
    osc0.setBounds(0, 0, 265, oscHeight);
    osc1.setBounds(0, oscHeight, 265, oscHeight);
    osc2.setBounds(0, oscHeight * 2, 265, oscHeight);
}
