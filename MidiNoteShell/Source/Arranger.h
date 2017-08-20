/*
  ==============================================================================

    Arranger.h
    Created: 2 May 2017 7:46:45pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Arranger    : public Component
{
public:
    Arranger();
    ~Arranger();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arranger)
};
