/*
  ==============================================================================

    RadialButtons.cpp
    Created: 30 Jan 2018 1:24:11pm
    Author:  jimi

  ==============================================================================
*/

#include "RadialButtons.h"

RadialButtons::RadialButtons (int enum_start, int enum_size, Font labelFont,
                              Justification labelJustification, Array<String> names,
                              int enum_default, Colour offColour, Colour onColour)
:   shortened (false),
    buttons (enum_size),
    labels (enum_size),
    originalNames (names)
{
    if (names.size() != enum_size) throw "Every radial button must have a name";
    if (enum_default == -1) enum_default = enum_start;
    start = enum_start;
    end = start + enum_size;
    for (int i = 0; i < enum_size; ++i) {
        addAndMakeVisible (buttons[i]);
        buttons[i].setColour (ToggleButton::tickColourId, onColour);
        buttons[i].setColour (ToggleButton::tickDisabledColourId, offColour);
        if (i == enum_default - enum_start) buttons[i].setToggleState (true, dontSendNotification);
        else buttons[i].setToggleState (false, dontSendNotification);
        addAndMakeVisible (labels[i]);
        labels[i].setText (names[i], dontSendNotification);
        labels[i].setFont (labelFont);
        labels[i].setJustificationType (labelJustification);
        //labels[i].attachToComponent (&(buttons[i]), false);
    }
}
RadialButtons::~RadialButtons()
{
}
void RadialButtons::addListenerToButtons (Button::Listener* newListener)
{
    for (int i = 0; i < buttons.size(); ++i)
        buttons[i].addListener (newListener);
}

int RadialButtons::getValue()
{
    int index = 0;
    for (; index < buttons.size(); ++index)
        if (buttons[index].getToggleState() == true) break;
    return index + start;
}
void RadialButtons::setValue (int value)
{
    int index = value - start;
    for (int i = 0; i < buttons.size(); ++i) {
        if (i == index) buttons[i].setToggleState (true, dontSendNotification);
        else buttons[i].setToggleState (false, dontSendNotification);
    }
}

bool RadialButtons::contains (Button* button)
{
    for (int i = 0; i < buttons.size(); ++i)
        if (button == &(buttons[i])) return true;
    return false;
}
int RadialButtons::clicked (Button* button)
{
    for (int i = 0; i < buttons.size(); ++i) {
        if (button == &(buttons[i])) {
            if (buttons[i].getToggleState() == false) setValue (i + start);
            return i + start;
        }
    }
    throw "Button not found";
}

void RadialButtons::paint (Graphics& g)
{
    // restore original label names
    /* somehow getting "error: no matching function for call to ‘juce::Label::setText(juce::String)’", which is an outright lie
    if (shortened) {
        shortened = false;
        for (int i = 0; i < labels.size(); ++i)
            labels[i].setText (originalNames[i]);
    }
    */
    Rectangle<int> area = getBounds();
    const int amount = end - start;
    const bool odd = amount % 2;
    const int border = 5;
    const int vborder = 5;
    const int buttonWidth = 90;
    const int buttonMargin = 20;
    const int buttonHeight = 20;
    const int labelWidth = buttonWidth - buttonMargin;
    if ((buttonWidth * 2 + border) <= area.getWidth()) {
        // 2 columns
        int currentColumn = 0;
        int currentRow = 0;
        int i = 0;
        if (odd) {
            // left-justify the first option and then start new row
            //const int totalWidth = buttonWidth * 2 + border * 3;
            //const int centerMargin = (totalWidth - buttonWidth) / 2;
            buttons[i].setBounds (0, 0, buttonWidth, buttonHeight);
            labels[i].setBounds (buttonMargin, 0, labelWidth, buttonHeight);
            ++currentRow; ++i;
        }
        for (; i < amount; ++i) {
            buttons[i].setBounds (currentColumn * (buttonWidth + border),
                                  currentRow * (buttonHeight + vborder),
                                  buttonWidth, buttonHeight);
            labels[i].setBounds (currentColumn * (buttonWidth + border) + buttonMargin,
                                  currentRow * (buttonHeight + vborder),
                                  labelWidth, buttonHeight);
            ++currentColumn;
            if (currentColumn > 1) {
                currentColumn = 0;
                ++currentRow;
            }
        }
    } else {
        // 1 column
        /* somehow getting "error: no matching function for call to ‘juce::Label::setText(juce::String)’", which is an outright lie
        if (buttonWidth > area.getWidth()) {
            // shorten label names to 2 characters
            shortened = true;
            for (Label* label : labels) {
                String name = label.getText();
                label.setText (name.substring (0, 2));
            }
        }
        */
        for (int i = 0; i < amount; ++i) {
            buttons[i].setBounds (0, i * (buttonHeight + vborder),
                                  buttonWidth, buttonHeight);
            labels[i].setBounds (buttonMargin, i * (buttonHeight + vborder),
                                  labelWidth, buttonHeight);
        }
    }
}
void RadialButtons::resized()
{
    repaint();
}
