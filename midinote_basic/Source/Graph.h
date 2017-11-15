/*
  ==============================================================================

    Graph.h
    Created: 14 Nov 2017 2:59:43pm
    Author:  jimi

  ==============================================================================
*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Graph    : public Component
{
public:
    Graph(Colour backgroundColour, Colour colour);
    ~Graph();

    void addPoint (Point<float> end);
    void addPoint (int x, int y);
    void clear();

    void setBackgroundColour (Colour colour);
    void setColour (Colour colour);

    bool isDrawn();
    bool toggleDrawn();
    void drawOn();
    void drawOff();

    void paint (Graphics& g) override;
    void resized() override;

protected:
    float startPoint;
    float endPoint;
    // so that we can do more advanced stuff later, because the Path
    // class does not allow stuff like inserting a new point in between
    // existing points:
    //Array<Point<float>> points;

private:
    Path path;
    Colour background;
    Colour colour;
    bool drawGraph; // for when we want the data but don't need a visual representation
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graph)
};

#endif  // GRAPH_H_INCLUDED
