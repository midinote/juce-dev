/*
  ==============================================================================

    MidiEditorGrid.h
    Created: 7 Apr 2017 12:49:21pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#ifndef MIDIEDITORGRID_H_INCLUDED
#define MIDIEDITORGRID_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MidiEditorGrid    : public Component,
                          public MidiKeyboardStateListener
{
public:
    MidiEditorGrid();
    ~MidiEditorGrid();

    void paint (Graphics&) override;
    void resized() override;
    void handleNoteOn(MidiKeyboardState*, int, int, float) override;
    void handleNoteOff(MidiKeyboardState*, int, int, float) override;

private:
    void logMessage (String m);
    
    TextEditor midiMessages;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEditorGrid)
};


#endif  // MIDIEDITORGRID_H_INCLUDED
