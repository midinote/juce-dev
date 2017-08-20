/*
  ==============================================================================

    MidiEditor.cpp
    Created: 6 Apr 2017 10:35:31am
    Author:  Malcolm Flint

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEditor.h"

//==============================================================================
MidiEditor::MidiEditor()
      : keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.    
    addAndMakeVisible (keyboardComponent);
    
    addAndMakeVisible (grid);
    keyboardState.addListener (&grid);
}

MidiEditor::~MidiEditor()
{
}

void MidiEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background
}

void MidiEditor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> area (getLocalBounds());
    int gridWidth = 210;
    int divider = area.getWidth() - gridWidth;
    keyboardComponent.setBounds(0, 0, divider, area.getHeight());
    grid.setBounds (divider, 0, gridWidth, area.getHeight());
    
}
