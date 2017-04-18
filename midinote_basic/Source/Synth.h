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
class Synth    : public Component,
                 public Slider::Listener
{
public:
    struct Settings {
        float A4Frequency;
        Oscillator::WaveType wave;
        float level;
        float pan;
    };

    Synth();
    ~Synth();

    std::pair<float,float> synthesize (double);
    void addNote (MidiMessage);
    void removeNote (MidiMessage);

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override;

    void updateSettings(Settings newSettings);
    void updateSettings(float A4Frequency, Oscillator::WaveType wave, float level, float pan);
    Settings* getSettings();

private:
    Settings settings;
    void updateKnobs();

    Oscillator::WaveType waveType;

    Slider frequencySlider;
    Label frequencyLabel;
    Slider levelSlider;
    Label levelLabel;
    Slider waveSlider;
    Label waveLabel;
    Slider panSlider;
    Label panLabel;
    std::map<int, std::pair<MidiMessage, Oscillator>> currentNotes;
    std::mutex lock;
    Font labelFont;
    Justification labelJustification;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)
};

#endif  // SYNTH_H_INCLUDED
