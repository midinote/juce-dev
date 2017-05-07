/*
  ==============================================================================

    Synth.cpp
    Created: 7 Apr 2017 7:05:20pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "Synth.h"


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
//    for (int i = 0; i < 3; i++) addWaveSound(WaveSound::sine, 1.0, 0.0, i);
}

void SynthAudioSource::addWaveSound(WaveSound::Wave wave, float level, float pan, int oscNum)
{
    ScopedPointer<WaveSound> sound = new WaveSound(level, pan, wave);
    sounds.insert(oscNum, sound);
}

void SynthAudioSource::applySounds ()
{
    synth.clearSounds();
    for (int i = 0; i < sounds.size(); ++i)
        synth.addSound(sounds[i]);
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
:   waveButtons("group", "Wave"),
    squareButton("square"),
    sineButton("sine"),
    sawtoothButton("sawtooth"),
    triangleButton("triangle"),
    noiseButton("noise"),
    waveType(WaveSound::sine),
    labelFont(Font (12)),
    labelJustification(Justification::centredTop)
{
    addAndMakeVisible(waveButtons);

    addAndMakeVisible(squareButton);
    squareButton.setClickingTogglesState(true);
    squareButton.setRadioGroupId(100);
    
    addAndMakeVisible(sineButton);
    sineButton.setClickingTogglesState(true);
    sineButton.setRadioGroupId(100);
    sineButton.setToggleState(true, dontSendNotification);
    
    addAndMakeVisible(sawtoothButton);
    sawtoothButton.setClickingTogglesState(true);
    sawtoothButton.setRadioGroupId(100);
    
    addAndMakeVisible(triangleButton);
    triangleButton.setClickingTogglesState(true);
    triangleButton.setRadioGroupId(100);
    
    addAndMakeVisible(noiseButton);
    noiseButton.setClickingTogglesState(true);
    noiseButton.setRadioGroupId(100);
    
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
    Rectangle<int> area = getBounds();
    int buttonHeight = 22;
    int buttonWidth = 100;
    int knobWidth = 65;
    int buttonGroupBorder = 10;
    int buttonGroupTopBorder = 20;
    
    Rectangle<int> buttonArea = area.removeFromLeft(buttonWidth);
    waveButtons.setBounds(buttonArea);
    buttonArea = buttonArea.withTrimmedTop(buttonGroupTopBorder);
    buttonArea = buttonArea.withTrimmedLeft(buttonGroupBorder);
    buttonArea = buttonArea.withTrimmedRight(buttonGroupBorder);
    
    squareButton.setBounds(buttonArea.removeFromTop(buttonHeight));
    sineButton.setBounds(buttonArea.removeFromTop(buttonHeight));
    sawtoothButton.setBounds(buttonArea.removeFromTop(buttonHeight));
    triangleButton.setBounds(buttonArea.removeFromTop(buttonHeight));
    noiseButton.setBounds(buttonArea.removeFromTop(buttonHeight));
    
    frequencySlider.setBounds (area.removeFromLeft(knobWidth));
    fineTuningSlider.setBounds (area.removeFromLeft(knobWidth));
    levelSlider.setBounds (area.removeFromLeft(knobWidth));
    panSlider.setBounds (area.removeFromLeft(knobWidth));
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
    : deviceManager (getSharedAudioDeviceManager()),
      synthAudioSource (keyboardState),
      keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(osc0);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(keyboardComponent);
    
    settings.osc0Settings = osc0.getSettings();
    settings.osc1Settings = osc1.getSettings();
    settings.osc2Settings = osc2.getSettings();
    audioSourcePlayer.setSource (&synthAudioSource);
    synthAudioSource.addWaveSound(settings.osc0Settings.wave,
                                  settings.osc0Settings.level,
                                  settings.osc0Settings.A4Frequency,
                                  settings.osc0Settings.pan);
    synthAudioSource.addWaveSound(settings.osc1Settings.wave,
                                  settings.osc1Settings.level,
                                  settings.osc1Settings.A4Frequency,
                                  settings.osc1Settings.pan);
    synthAudioSource.addWaveSound(settings.osc2Settings.wave,
                                  settings.osc2Settings.level,
                                  settings.osc2Settings.A4Frequency,
                                  settings.osc2Settings.pan);
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
    Rectangle<int> area = getBounds();
    int oscHeight = 140;
    int keyboardHeight = 100;
    osc0.setBounds(area.removeFromTop(oscHeight));
    osc1.setBounds(area.removeFromTop(oscHeight));
    osc2.setBounds(area.removeFromTop(oscHeight));
    keyboardComponent.setBounds(area.removeFromBottom(keyboardHeight));
}
