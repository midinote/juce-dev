/*
  ==============================================================================

    MainArea.h
    Created: 3 May 2017 4:25:18pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Arranger.h"
#include "Editor.h"
#include "FileExplorer.h"
#include "Chat.h"

//==============================================================================
/*
*/
class MainArea    : public Component
{
public:
    MainArea();
    ~MainArea();

    void paint (Graphics&) override;
    void resized() override;

private:
    Arranger arranger;
    Editor editor;
    FileExplorer fileExplorer;
    Chat chat;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainArea)
};
