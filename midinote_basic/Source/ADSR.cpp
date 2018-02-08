/*
  ==============================================================================

    ADSR.cpp
    Created: 7 Feb 2018 2:15:38pm
    Author:  jimi

  ==============================================================================
*/

#include "ADSR.h"

ADSR::ADSR (Colour backgroundColour, Colour colour) : Graph(backgroundColour, colour)
{
    // defaults
    maxMS = 2000;
    maxdB = 100;
    startPointMS = 0;
    sustainLength = 200.0; // how much X distance between decay & sustain points on graph
    setAttack (Point<float> (100.0, static_cast<float> (maxdB)));
    setDecay (Point<float> (400.0, static_cast<float> (maxdB / 2)));
    setSustain (Point<float> (getDecay() + sustainLength, static_cast<float> (maxdB / 2)));
    setRelease (1200.0); // this sets endPointMS
}

ADSR::~ADSR()
{
}

float ADSR::getStartPoint()
{
    return startPointMS;
}
int ADSR::getMaxMS()
{
    return maxMS;
}
int ADSR::getMaxdB()
{
    return maxdB;
}

float ADSR::getAttack()
{
    return attack.getX();
}
Point<float> ADSR::getAttackGraph()
{
    return attack;
}

float ADSR::getDecay()
{
    return decay.getX();
}
Point<float> ADSR::getDecayGraph()
{
    //auto adjustedPoint = Point<float> (decay.getX() + attack.getX(), decay.getY());
    return decay;
}

float ADSR::getSustain()
{
    return sustain.getY();
}
Point<float> ADSR::getSustainGraph()
{
    //auto adjustedPoint = Point<float> (sustain.getX() + decay.getX() + attack.getX(),
    //                                   decay.getY());
    return sustain;
}

float ADSR::getRelease()
{
    return endPointMS;
}
Point<float> ADSR::getReleaseGraph()
{
    //return Point<float> (endPointMS + sustain.getX() + decay.getX() + attack.getX(), 0.0);
    return Point<float> (endPointMS, 0.0);
}

void ADSR::redraw()
{
    clear();
    // use multiplication to make the bounds of the graph represent the
    // bounds of the miliseconds and decibels
    Rectangle<int> area = getLocalBounds();
    float Xmult = static_cast<float> (area.getWidth()) / static_cast<float> (maxMS);
    float Ymult = static_cast<float> (area.getHeight()) / static_cast<float> (maxdB);
    startPoint = startPointMS * Xmult;
    endPoint = getReleaseGraph().getX() * Xmult;
    auto attackPoint = Point<float> (getAttackGraph().getX() * Xmult,
                                     static_cast<float> (area.getHeight()) - attack.getY() * Ymult);
    auto decayPoint = Point<float> (getDecayGraph().getX() * Xmult,
                                    static_cast<float> (area.getHeight()) - decay.getY() * Ymult);
    auto sustainPoint = Point<float> (getSustainGraph().getX() * Xmult,
                                      static_cast<float> (area.getHeight()) - sustain.getY() * Ymult);
    addPoint (attackPoint);
    addPoint (decayPoint);
    addPoint (sustainPoint);
    repaint();
}

void ADSR::setAttack (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - attack.getX();
    attack = point;
    //decay.setX (decay.getX() + diff);
    //sustain.setX (sustain.getX() + diff);
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getAttackGraph().getX());
    if (sliderY != nullptr) sliderY->setValue (getAttackGraph().getY());
    redraw();
}
void ADSR::setAttack (float MS, Slider* sliderX)
{
    setAttack (Point<float> (MS, getAttackGraph().getY()), sliderX, nullptr);
}

void ADSR::setDecay (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - decay.getX();
    //auto adjustedPoint = Point<float> (point.getX() - attack.getX(), point.getY());
    decay = point;
    //sustain.setX (sustain.getX() + diff);
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getDecayGraph().getX());
    if (sliderY != nullptr) sliderY->setValue (getDecayGraph().getY());
    redraw();
}
void ADSR::setDecay (float MS, Slider* sliderX)
{
    float differenceMS = MS - getDecayGraph().getX();
    setDecay (Point<float> (MS, getDecayGraph().getY()), sliderX, nullptr);
    setSustain (Point<float> (MS + sustainLength, getSustainGraph().getY()), nullptr, nullptr);
    setRelease (getRelease() + differenceMS, nullptr);
}

void ADSR::setSustain (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - sustain.getX();
    //auto adjustedPoint = Point<float> (point.getX() - attack.getX() - decay.getX(),
    //                                   point.getY());
    sustain = point;
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getSustainGraph().getX());
    if (sliderY != nullptr) sliderY->setValue (getSustainGraph().getY());
    redraw();
}
void ADSR::setSustain (float dB, Slider* sliderY)
{
    setSustain (Point<float> (getSustainGraph().getX(), dB), nullptr, sliderY);
    setDecay (Point<float> (getDecayGraph().getX(), dB), nullptr, nullptr);
}

void ADSR::setRelease (Point<float> point, Slider* sliderX)
{
    setRelease (point.getX(), sliderX);
}
void ADSR::setRelease (float MS, Slider* sliderX)
{
    //endPointMS = x - sustain.getX() - decay.getX() - attack.getX();
    endPointMS = MS;
    if (sliderX != nullptr) sliderX->setValue (getReleaseGraph().getX());
    redraw();
}

void ADSR::resized()
{
    Graph::resized();
    redraw();
}
