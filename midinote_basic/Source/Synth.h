/*
  ==============================================================================

    Synth.h
    Created: 7 Apr 2017 7:05:20pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#ifndef SYNTH_H_INCLUDED
#define SYNTH_H_INCLUDED

#include <map>
#include <mutex>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

//==============================================================================
/*
*/
class SynthAudioSource : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState);
    
    void addWaveSound (WaveSound::Wave wave, float level, float pan, int oscNum);
    void applySounds();
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override;
    void releaseResources () override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    MidiMessageCollector midiCollector;
private:
    std::vector<WaveSound*> sounds;
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
};

class OscillatorComponent    : public Component,
                               public Slider::Listener,
                               public Button::Listener
{
public:
    struct Settings {
        float A4Frequency;
        WaveSound::Wave wave;
        float level;
        float pan;
    };

    OscillatorComponent ();
    ~OscillatorComponent ();

    void paint (Graphics& g) override;
    void resized () override;
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button* button) override;

    void updateSettings(Settings newSettings);
    void updateSettings(float A4Frequency, WaveSound::Wave wave, float level, float pan);
    Settings* getSettings();
    
    Slider frequencySlider;
    Label frequencyLabel;
    Slider fineTuningSlider;
    Label fineTuningLabel;
    Slider levelSlider;
    Label levelLabel;
    Slider panSlider;
    Label panLabel;
    TextButton squareButton;
    TextButton sineButton;
    TextButton sawtoothButton;
    TextButton triangleButton;
    TextButton noiseButton;
    

private:
    Settings settings;
    void updateKnobs();
    std::map<WaveSound::Wave, Slider*> waveSliderMap;
    WaveSound::Wave waveType;

    Font labelFont;
    Justification labelJustification;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)
};

class SynthComponent    : public Component
{
public:
    struct settings {
        OscillatorComponent::Settings osc0Settings;
        OscillatorComponent::Settings osc1Settings;
        OscillatorComponent::Settings osc2Settings;
    };
    SynthComponent ();
    ~SynthComponent ();
    void paint (Graphics& g) override;
    void resized () override;
private:
    OscillatorComponent osc0, osc1, osc2;
    AudioDeviceManager& deviceManager;
    MidiKeyboardState keyboardState;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource;
    MidiKeyboardComponent keyboardComponent;
};


#endif  // SYNTH_H_INCLUDED
