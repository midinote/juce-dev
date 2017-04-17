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
    Synth();
    ~Synth();
    
    float synthesize (double);
    void addNote (MidiMessage);
    void removeNote (MidiMessage);

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override;

private:
    Oscillator::WaveType waveType;

    Slider frequencySlider;
    Label frequencyLabel;
    Slider levelSlider;
    Label levelLabel;
    Slider panSlider;
    Label panLabel;
    std::map<int, std::pair<MidiMessage, Oscillator>> currentNotes;
    std::mutex lock;
    
    float pan;
    double currentFrequency;
    bool noteOn;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)
};

#endif  // SYNTH_H_INCLUDED
