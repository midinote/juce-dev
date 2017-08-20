/*
  ==============================================================================

    MainArea.cpp
    Created: 3 May 2017 4:25:18pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainArea.h"

//==============================================================================
MainArea::MainArea()
{
    addAndMakeVisible(arranger);
    arranger.setBounds(0, 0, getWidth(), getHeight());
    addAndMakeVisible(editor);
    addAndMakeVisible(fileExplorer);
    addAndMakeVisible(chat);
}

MainArea::~MainArea()
{
}

void MainArea::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("MainArea", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void MainArea::resized()
{
    repaint();
    arranger.setBounds(arranger.getX(),
                       arranger.getY(),
                       arranger.getWidth(),
                       arranger.getHeight());
}
