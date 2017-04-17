/*
  ==============================================================================

    MidiEditor.h
    Created: 6 Apr 2017 10:35:31am
    Author:  Malcolm Flint

  ==============================================================================
*/

#ifndef MIDIEDITOR_H_INCLUDED
#define MIDIEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEditorGrid.h"


//==============================================================================
/*
*/
class MidiEditor    : public Component
{
public:
    MidiEditor();
    ~MidiEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    MidiKeyboardState keyboardState;


private:
    MidiKeyboardComponent keyboardComponent;
    MidiEditorGrid grid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEditor)
};


#endif  // MIDIEDITOR_H_INCLUDED
