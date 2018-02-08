/*
  ==============================================================================

    ADSR.h
    Created: 7 Feb 2018 2:15:38pm
    Author:  jimi

  ==============================================================================
*/

#ifndef ADSR_H_INCLUDED
#define ADSR_H_INCLUDED

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
    float getAttack();
    Point<float> getAttackGraph();
    float getDecay();
    Point<float> getDecayGraph();
    float getSustain();
    Point<float> getSustainGraph();
    float getRelease();
    Point<float> getReleaseGraph();
    // their sliders can also be updated along with the graph if passed in
    void setAttack (Point<float> point, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setAttack (float MS, Slider* sliderX = nullptr);
    void setDecay (Point<float> point, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setDecay (float MS, Slider* sliderX = nullptr);
    void setSustain (Point<float> point, Slider* sliderX = nullptr, Slider* sliderY = nullptr);
    void setSustain (float dB, Slider* sliderY = nullptr);
    void setRelease (Point<float> point, Slider* sliderX = nullptr);
    void setRelease (float MS, Slider* sliderX = nullptr);
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
    float sustainLength;

    void redraw(); // needed because a path has to be redrawn to insert
                   // a point between two other points
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSR)
};

#endif  // ADSR_H_INCLUDED
