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

#endif  // ADSR_H_INCLUDED
