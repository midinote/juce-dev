/*
  ==============================================================================

    FileExplorer.h
    Created: 2 May 2017 7:53:40pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FileExplorer    : public Component
{
public:
    FileExplorer();
    ~FileExplorer();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileExplorer)
};
