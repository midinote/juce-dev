/*
  ==============================================================================

    Chat.h
    Created: 2 May 2017 7:55:23pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Chat    : public Component
{
public:
    Chat();
    ~Chat();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Chat)
};
