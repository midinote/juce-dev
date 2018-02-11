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
#include "Network.h"
#include "Oscillator.h"
#include "ADSR.h"
#include "RadialButtons.h"
#include "Filter.h"

//==============================================================================
/*
*/

class Synth    : public Component,
                 public Slider::Listener,
                 public Button::Listener,
				 public Transmittable
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

    void sliderValueChanged (Slider*) override;
    void buttonClicked (Button* button) override;

    void updateADSR (Point<float> attack, Point<float> decay, Point<float> sustain, float release);
    void updateADSR (float attack, float decay, float sustain, float release);
    void updateADSRKnobs();

    void updateSettings (Settings newSettings);
    void updateSettings (float A4Frequency, Oscillator::WaveType wave, float level, float pan);
    Settings* getSettings();

	// Inherited via Transmittable
	virtual void updateTree(ValueTree & t) override;
	virtual void updateValues(ValueTree & t) override;

    Slider frequencySlider;
    Label frequencyLabel;
    Slider levelSlider;
    Label levelLabel;
    Slider panSlider;
    Label panLabel;
    RadialButtons waveButtons;
    Label waveLabel;

    ComboBox envelopeMenu;
    ADSR envelopeADSR;
    Slider attackSlider;
    Label attackLabel;
    Slider decaySlider;
    Label decayLabel;
    Slider sustainSlider;
    Label sustainLabel;
    Slider releaseSlider;
    Label releaseLabel;

    Filter filter;
    Label filterModeLabel;
    Slider cutoffSlider;
    Label cutoffLabel;
    Slider resonanceSlider;
    Label resonanceLabel;

private:
    Settings settings;
    void updateKnobs();

    bool updateADSRLock;

    Oscillator::WaveType waveType;

    std::map<int, std::pair<MidiMessage, Oscillator>> currentNotes;
    std::mutex lock;
    Font labelFont;
    Justification labelJustification;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)

};

#endif  // SYNTH_H_INCLUDED
