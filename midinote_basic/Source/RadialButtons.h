/*
  ==============================================================================

    RadialButtons.h
    Created: 30 Jan 2018 1:24:10pm
    Author:  jimi

    Turns an enum into a radial menu that automatically attempts to fill
    the space it's in. It assumes the enum doesn't have any gaps in its number
    assignments.
    Currently does not support having more than one bubble selected at a time.
  ==============================================================================
*/

#ifndef RADIALBUTTONS_H_INCLUDED
#define RADIALBUTTONS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class RadialButtons : public Component
{
public:
    RadialButtons (int enum_start, int enum_size, Font labelFont,
                   Justification labelJustification, Array<String> names,
                   int enum_default = -1, Colour offColour = Colour (36, 36, 36), // dark gray
                   Colour onColour = Colour (66, 162, 200)); // cool blue
    ~RadialButtons();
    void addListenerToButtons (Button::Listener *newListener);

    int getValue();
    void setValue (int value);

    bool contains (Button* button);
    int clicked (Button* button);

    void paint (Graphics& g) override;
    void resized() override;

private:
    int start;
    int end;
    int current;
    // have to use vectors because Juce::Array does not have an "initialize with this amount of empty elements" constructor
    std::vector<ToggleButton> buttons;
    std::vector<Label> labels;
    bool shortened;
    Array<String> originalNames;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadialButtons)
};

#endif  // RADIALBUTTONS_H_INCLUDED
