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
#include "Graph.h"

//==============================================================================
/*
*/

class ADSR     : public Graph
{
public:
    ADSR(Colour backgroundColour = Colour (36, 36, 36), // dark gray
         Colour colour = Colour (66, 162, 200)); // cool blue
    ~ADSR();

    Point<float> getAttack();
    Point<float> getDecay();
    Point<float> getSustain();
    Point<float> getRelease();
    void setAttack (Point<float> point);
    void setAttack (int x, int y);
    void setDecay (Point<float> point);
    void setDecay (int x, int y);
    void setSustain (Point<float> point);
    void setSustain (int x, int y);
    void setRelease (Point<float> point);
    void setRelease (int x, int y);

private:
    // stored in miliseconds and decibels; converted to pixels in redraw()
    // keep in mind that means the Y axis increases when going up--the
    // opposite of what pixel values do
    Point<float> attack;
    Point<float> decay;
    Point<float> sustain;
    // release (x,y) is just the endpoint and the bottom of the graph

    void redraw(); // needed because a path has to be redrawn to insert
                   // a point between two other points
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSR)
};

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

    Slider frequencySlider;
    Label frequencyLabel;
    Slider levelSlider;
    Label levelLabel;
    Slider panSlider;
    Label panLabel;
    Slider waveSlider;
    Label waveLabel;

    ComboBox envelopeMenu;
    ADSR envelopeADSR;

private:
    Settings settings;
    void updateKnobs();

    Oscillator::WaveType waveType;

    std::map<int, std::pair<MidiMessage, Oscillator>> currentNotes;
    std::mutex lock;
    Font labelFont;
    Justification labelJustification;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)
};

#endif  // SYNTH_H_INCLUDED
