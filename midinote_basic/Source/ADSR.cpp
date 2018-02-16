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
    endPointMS = 900.0;
    sustainLength = endPointMS / 10.0;
    attack = Point<float> (100.0, static_cast<float> (maxdB));
    actualDecay = 300.0;
    decay = Point<float> (attack.getX() + actualDecay, static_cast<float> (maxdB / 2));
    sustain = Point<float> (getDecay() + sustainLength, static_cast<float> (maxdB / 2));
    actualRelease = endPointMS - actualDecay;
    redrawSustainAndRelease();
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
    return actualDecay;
}
Point<float> ADSR::getDecayGraph()
{
    return decay;
}

float ADSR::getSustain()
{
    return sustain.getY();
}
Point<float> ADSR::getSustainGraph()
{
    return sustain;
}

float ADSR::getRelease()
{
    return actualRelease;
}
Point<float> ADSR::getReleaseGraph()
{
    return Point<float> (endPointMS, 0.0);
}

void ADSR::redrawSustainAndRelease()
{
    decay.setX (getAttackGraph().getX() + actualDecay);
    endPointMS = getRelease() + getSustainGraph().getX();
    sustainLength = endPointMS / 10.0;
    sustain.setX (getDecayGraph().getX() + sustainLength);
    endPointMS = getRelease() + getSustainGraph().getX();
    redraw();
}

void ADSR::redraw()
{
    clear();
    // use multiplication to make the bounds of the graph represent the
    // bounds of the miliseconds and decibels
    Rectangle<int> area = getLocalBounds();
    // very weird bug: area.getWidth() is lying, and this math corrects it
    // 884 is the width getWidth() returns when the window is 609 (currently Main.cpp:MIN_WIDTH)
    // 142 is the amount to subtract that by to get an ADSR graph that actually fits
    // sustainLength definitely has something to do with this math, but somehow just subtracting
    // sustainLength from area.getWidth() does not solve this
    const double error = 142.0 / 884.0;
    const float width = static_cast<float> (area.getWidth());
    const double Xmult = (width - (width * error)) / static_cast<float> (maxMS);
    const float Ymult = static_cast<float> (area.getHeight()) / static_cast<float> (maxdB);
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
    attack = point;
    if (sliderX != nullptr) sliderX->setValue (getAttackGraph().getX());
    if (sliderY != nullptr) sliderY->setValue (getAttackGraph().getY());
    redrawSustainAndRelease();
}
void ADSR::setAttack (float MS, Slider* sliderX)
{
    setAttack (Point<float> (MS, getAttackGraph().getY()), sliderX, nullptr);
}

void ADSR::setDecay (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    decay = point;
    if (sliderX != nullptr) sliderX->setValue (getDecayGraph().getX());
    if (sliderY != nullptr) sliderY->setValue (getDecayGraph().getY());
    redrawSustainAndRelease();
}
void ADSR::setDecay (float MS, Slider* sliderX)
{
    actualDecay = MS;
    setDecay (Point<float> (MS + getAttack(), getDecayGraph().getY()), sliderX, nullptr);
}

void ADSR::setSustain (Point<float> point, Slider* sliderX, Slider* sliderY,
                       bool calledFromOtherSetFunction)
{
    sustain = point;
    if (sliderX != nullptr) sliderX->setValue (getSustainGraph().getX());
    if (sliderY != nullptr) sliderY->setValue (getSustainGraph().getY());
    if (not calledFromOtherSetFunction) redrawSustainAndRelease();
}
void ADSR::setSustain (float dB, Slider* sliderY)
{
    setSustain (Point<float> (getDecayGraph().getX() + sustainLength, dB),
                nullptr, sliderY, true);
    setDecay (Point<float> (getDecayGraph().getX(), dB), nullptr, nullptr);
}

void ADSR::setRelease (Point<float> point, Slider* sliderX)
{
    setRelease (point.getX(), sliderX);
}
void ADSR::setRelease (float MS, Slider* sliderX)
{
    actualRelease = MS;
    if (sliderX != nullptr) sliderX->setValue (getRelease());
    redrawSustainAndRelease();
}

void ADSR::resized()
{
    Graph::resized();
    redraw();
}
