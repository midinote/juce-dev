/*
  ==============================================================================

    Graph.cpp
    Created: 14 Nov 2017 2:59:43pm
    Author:  jimi

  ==============================================================================
*/

#include "Graph.h"

Graph::Graph (Colour backgroundColour, Colour colour)
{
    drawGraph = false;
    background = backgroundColour;
    this->colour = colour;
    startPoint = 0.0;
    endPoint = 0.0;
}

Graph::~Graph()
{
}

void Graph::addPoint (Point<float> point)
{
    Rectangle<int> area = getBounds();
    if (path.isEmpty()) {
        Point<float> firstPoint = Point<float> (startPoint,
                                                static_cast<float> (area.getHeight()));
        path.startNewSubPath (firstPoint);
        //points.add (firstPoint);
    }
    path.lineTo (point);
    //points.add (point);
}

void Graph::addPoint (int x, int y)
{
    addPoint (Point<float> (static_cast<float> (x), static_cast<float> (y)));
}

void Graph::clear()
{
    path.clear();
}

void Graph::setBackgroundColour (Colour colour)
{
    background = colour;
}

void Graph::setColour (Colour colour)
{
    this->colour = colour;
}

bool Graph::isDrawn()
{
    return drawGraph;
}

bool Graph::toggleDrawn()
{
    drawGraph = !drawGraph;
    return drawGraph;
}

void Graph::drawOn()
{
    drawGraph = true;
}

void Graph::drawOff()
{
    drawGraph = false;
}


float smallerNumber (float a, float b)
{
    return a < b ? a : b;
}

void Graph::paint (Graphics& g)
{
    if (drawGraph) {
        g.fillAll (background);
        // copy path to a new, prettier-looking path that extends to the bottom
        // and fill it in with color
        Rectangle<int> area = getBounds();
        Rectangle<float> sizeOfGraph = path.getBounds();
        float smallestBound = smallerNumber (sizeOfGraph.getWidth(), sizeOfGraph.getHeight());
        float cornerRadius = smallestBound / 20.0;
        path.lineTo (endPoint, static_cast<float> (area.getHeight()));
        auto fill = path.createPathWithRoundedCorners (cornerRadius);
        // don't worry, it's better if the bottom points aren't rounded
        fill.closeSubPath();
        g.setColour (colour);
        g.fillPath (fill);
    }
}

void Graph::resized()
{
    Rectangle<int> area = getBounds();
    endPoint = static_cast<float> (area.getWidth()) - endPoint;
}
