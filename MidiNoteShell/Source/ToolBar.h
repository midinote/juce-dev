/*
  ==============================================================================

    ToolBar.h
    Created: 2 May 2017 7:45:59pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ToolBar    : public Component
{
public:
    ToolBar();
    ~ToolBar();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolBar)
};
