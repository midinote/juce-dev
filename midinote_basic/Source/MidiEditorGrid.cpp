/*
  ==============================================================================

    MidiEditorGrid.cpp
    Created: 7 Apr 2017 12:49:21pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "MidiEditorGrid.h"

//==============================================================================
MidiEditorGrid::MidiEditorGrid()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(midiMessages);
    midiMessages.setFont(Font(12));
    midiMessages.setMultiLine (true);
    midiMessages.setReturnKeyStartsNewLine (true);
    midiMessages.setReadOnly (true);
    midiMessages.setScrollbarsShown (true);
    midiMessages.setCaretVisible (false);
    midiMessages.setPopupMenuEnabled (true);
    midiMessages.setColour (TextEditor::backgroundColourId, Colour (0x94000000));
    midiMessages.setColour (TextEditor::outlineColourId, Colour (0x1c000000));
    midiMessages.setColour (TextEditor::shadowColourId, Colour (0x16000000));
}

MidiEditorGrid::~MidiEditorGrid()
{
}

void MidiEditorGrid::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
}

void MidiEditorGrid::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    midiMessages.setBounds(getLocalBounds());
}

void MidiEditorGrid::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage m (MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity));
    m.setTimeStamp(Time::currentTimeMillis());
    logMessage(m.getDescription());
}

void MidiEditorGrid::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    MidiMessage m (MidiMessage::noteOff(midiChannel, midiNoteNumber));
    m.setTimeStamp(Time::currentTimeMillis());
    logMessage(m.getDescription());
}


void MidiEditorGrid::logMessage (String m)
{
    midiMessages.moveCaretToEnd();
    midiMessages.insertTextAtCaret (m + newLine);
}
