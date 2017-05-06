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
#include "MidiNoteFunctionLib.h"
#include "AudioDeviceManagement.h"

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
    Array<WaveSound*> sounds;
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
    Settings getSettings();
    
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
    WaveSound::Wave waveType;

    Font labelFont;
    Justification labelJustification;
};

class SynthComponent    : public Component
{
public:
    struct Settings {
        OscillatorComponent::Settings osc0Settings;
        OscillatorComponent::Settings osc1Settings;
        OscillatorComponent::Settings osc2Settings;
    };
    SynthComponent ();
    ~SynthComponent ();
    Settings* getSettings();
    void updateSettings(Settings newSettings);
    void paint (Graphics& g) override;
    void resized () override;
private:
    OscillatorComponent osc0, osc1, osc2;
    AudioDeviceManager& deviceManager;
    MidiKeyboardState keyboardState;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource;
    MidiKeyboardComponent keyboardComponent;
    Settings settings;
//    Array<Oscillator> oscillators;
};


#endif  // SYNTH_H_INCLUDED
