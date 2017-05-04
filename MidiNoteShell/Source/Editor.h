/*
  ==============================================================================

    Editor.h
    Created: 2 May 2017 7:51:20pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Editor    : public Component
{
public:
    Editor();
    ~Editor();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

