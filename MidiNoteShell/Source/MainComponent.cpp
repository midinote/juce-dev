/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
    : editorIsVisible(true),
      fileExplorerIsVisible(true),
      toolbarHeight(40),
      editorHeight(200),
      editorCollapserHeight(10),
      editorCollapserWidth(10),
      fileExplorerWidth(200),
      fileExplorerCollapserWidth(10),
      collapseButtonImageUp(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("upButton.png")))),
      collapseButtonImageDown(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("downButton.png")))),
      collapseButtonImageLeft(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("rightButton.png")))),
      collapseButtonImageRight(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("leftButton.png"))))
{
    
    addAndMakeVisible(toolbar);
    
    addAndMakeVisible(arranger);
    
    addAndMakeVisible(editor);
    addAndMakeVisible(editorCollapser);
    editorCollapser.setImages(true, true, false,
                              collapseButtonImageUp, 0.5f, Colour(9,60,66),
                              collapseButtonImageUp, 1.0f, Colour(9,60,66),
                              collapseButtonImageDown, 0.5f, Colour(9,60,66),
                              0.5f);
    editorCollapser.addListener(this);
    
    addAndMakeVisible(fileExplorer);
    addAndMakeVisible(fileExplorerCollapser);
        
    setSize (1280, 714);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    repaint();
    Rectangle<int> area = getBounds();
    
    toolbar.setBounds(area.removeFromTop (toolbarHeight));
    if (editorIsVisible) editor.setBounds(area.removeFromBottom(editorHeight));
    if (fileExplorerIsVisible) fileExplorer.setBounds(area.removeFromRight(fileExplorerWidth));
    arranger.setBounds(area);
    editorCollapser.setBounds(area.getX(),
                              area.getY(),
                              editorCollapserWidth,
                              editorCollapserHeight);
}

void MainContentComponent::buttonClicked(Button* button)
{
    
}
