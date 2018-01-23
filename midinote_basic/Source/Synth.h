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

    float getStartPoint();
    int getMaxMS();
    int getMaxdB();
    Point<float> getAttack();
    Point<float> getDecay();
    Point<float> getSustain();
    Point<float> getRelease();
    // their sliders can also be updated along with the graph if passed in
    void setAttack (Point<float> point, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setAttack (int x, int y, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setAttackX (int x, Slider* sliderX = nullptr);
    void setAttackY (int y, Slider* sliderY = nullptr);
    void setDecay (Point<float> point, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setDecay (int x, int y, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setDecayX (int x, Slider* sliderX = nullptr);
    void setDecayY (int y, Slider* sliderY = nullptr);
    void setSustain (Point<float> point, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setSustain (int x, int y, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setSustainX (int x, Slider* sliderX = nullptr);
    void setSustainY (int y, Slider* sliderY = nullptr);
    void setReleaseX (float x, Slider* sliderX = nullptr);
    void setReleaseX (Point<float> point, Slider* sliderX = nullptr);
    //void setReleaseX (int x, Slider* sliderX = nullptr); this one causes ambiguous overload

    void resized() override;

private:
    // stored in miliseconds and decibels; converted to pixels in redraw()
    // keep in mind that means the Y axis increases when going up--the
    // opposite of what pixel values do
    int maxMS; // how many miliseconds the width of the graph represents
    int maxdB; // how many decibels the height of the graph represents
    float startPointMS; // Graph::startPoint in miliseconds
    float endPointMS; // Graph::endPoint in miliseconds
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
    void sliderValueChanged (Slider*) override;

    void updateADSR (Point<float> attack, Point<float> decay, Point<float> sustain, float release);
    void updateADSRKnobs();

    void updateSettings (Settings newSettings);
    void updateSettings (float A4Frequency, Oscillator::WaveType wave, float level, float pan);
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
    Slider attackSliderX;
    Label attackLabelX;
    Slider attackSliderY;
    Label attackLabelY;
    Slider decaySliderX;
    Label decayLabelX;
    Slider decaySliderY;
    Label decayLabelY;
    Slider sustainSliderX;
    Label sustainLabelX;
    Slider sustainSliderY;
    Label sustainLabelY;
    Slider releaseSliderX;
    Label releaseLabelX;

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
