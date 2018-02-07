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
    maxMS = 10000;
    maxdB = 60;
    startPointMS = 0;
    setAttack (1000, 50);
    setDecay (1500, 30);
    setSustain (6000, 30);
    setReleaseX (6500); // this sets endPointMS
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

Point<float> ADSR::getAttack()
{
    return attack;
}
Point<float> ADSR::getDecay()
{
    //auto adjustedPoint = Point<float> (decay.getX() + attack.getX(), decay.getY());
    return decay;
}
Point<float> ADSR::getSustain()
{
    //auto adjustedPoint = Point<float> (sustain.getX() + decay.getX() + attack.getX(),
    //                                   decay.getY());
    return sustain;
}
Point<float> ADSR::getRelease()
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
    endPoint = getRelease().getX() * Xmult;
    auto attackPoint = Point<float> (getAttack().getX() * Xmult,
                                     static_cast<float> (area.getHeight()) - attack.getY() * Ymult);
    auto decayPoint = Point<float> (getDecay().getX() * Xmult,
                                    static_cast<float> (area.getHeight()) - decay.getY() * Ymult);
    auto sustainPoint = Point<float> (getSustain().getX() * Xmult,
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
    if (sliderX != nullptr) sliderX->setValue (getAttack().getX());
    if (sliderY != nullptr) sliderY->setValue (getAttack().getY());
    redraw();
}
void ADSR::setAttack (int x, int y, Slider* sliderX, Slider* sliderY)
{
    setAttack (Point<float> (static_cast<float> (x), static_cast<float> (y)), sliderX, sliderY);
}
void ADSR::setAttackX (int x, Slider* sliderX)
{
    setAttack (Point<float> (static_cast<float> (x), attack.getY()), sliderX, nullptr);
}
void ADSR::setAttackY (int y, Slider* sliderY)
{
    setAttack (Point<float> (attack.getX(), static_cast<float> (y)), nullptr, sliderY);
}

void ADSR::setDecay (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - decay.getX();
    //auto adjustedPoint = Point<float> (point.getX() - attack.getX(), point.getY());
    decay = point;
    //sustain.setX (sustain.getX() + diff);
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getDecay().getX());
    if (sliderY != nullptr) sliderY->setValue (getDecay().getY());
    redraw();
}
void ADSR::setDecay (int x, int y, Slider* sliderX, Slider* sliderY)
{
    setDecay (Point<float> (static_cast<float> (x), static_cast<float> (y)), sliderX, sliderY);
}
void ADSR::setDecayX (int x, Slider* sliderX)
{
    setDecay (Point<float> (static_cast<float> (x), decay.getY()), sliderX, nullptr);
}
void ADSR::setDecayY (int y, Slider* sliderY)
{
    setDecay (Point<float> (decay.getX(), static_cast<float> (y)), nullptr, sliderY);
}

void ADSR::setSustain (Point<float> point, Slider* sliderX, Slider* sliderY)
{
    //float diff = point.getX() - sustain.getX();
    //auto adjustedPoint = Point<float> (point.getX() - attack.getX() - decay.getX(),
    //                                   point.getY());
    sustain = point;
    //endPointMS += diff;
    if (sliderX != nullptr) sliderX->setValue (getSustain().getX());
    if (sliderY != nullptr) sliderY->setValue (getSustain().getY());
    redraw();
}
void ADSR::setSustain (int x, int y, Slider* sliderX, Slider* sliderY)
{
    setSustain (Point<float> (static_cast<float> (x), static_cast<float> (y)), sliderX, sliderY);
}
void ADSR::setSustainX (int x, Slider* sliderX)
{
    setSustain (Point<float> (static_cast<float> (x), sustain.getY()), sliderX, nullptr);
}
void ADSR::setSustainY (int y, Slider* sliderY)
{
    setSustain (Point<float> (sustain.getX(), static_cast<float> (y)), nullptr, sliderY);
}

void ADSR::setReleaseX (float x, Slider* sliderX)
{
    //endPointMS = x - sustain.getX() - decay.getX() - attack.getX();
    endPointMS = x;
    if (sliderX != nullptr) sliderX->setValue (getRelease().getX());
    redraw();
}
void ADSR::setReleaseX (Point<float> point, Slider* sliderX)
{
    setReleaseX (point.getX(), sliderX);
}
/* this one causes ambiguous overload
void ADSR::setReleaseX (int x, Slider* sliderX)
{
    setReleaseX (static_cast<float> (x), sliderX);
}
*/

void ADSR::resized()
{
    Graph::resized();
    redraw();
}
