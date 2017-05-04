/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
: editorCollapser("Editor", 0.75f, Colours::black),
      fileExplorerCollapser("File Explorer", 0.5f, Colours::black),
      editorIsVisible(true),
      fileExplorerIsVisible(true),
      toolbarHeight(40),
      editorHeight(200),
      fileExplorerWidth(200),
      collapseButtonWidth(15),
      collapseButtonImageUp(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("upButton.png")))),
      collapseButtonImageDown(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("downButton.png")))),
      collapseButtonImageLeft(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("rightButton.png")))),
      collapseButtonImageRight(PNGImageFormat().loadFrom(File(File::getCurrentWorkingDirectory().getChildFile ("leftButton.png"))))
{
    
    addAndMakeVisible(toolbar);
    
    addAndMakeVisible(arranger);
    
    addAndMakeVisible(editor);
    addAndMakeVisible(editorCollapser);
//    editorCollapser.setImages(true, true, false,
//                              collapseButtonImageUp, 0.5f, Colour(9,60,66),
//                              collapseButtonImageUp, 1.0f, Colour(9,60,66),
//                              collapseButtonImageDown, 0.5f, Colour(9,60,66),
//                              0.5f);
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
    Rectangle<int> buttonRect (collapseButtonWidth, collapseButtonWidth);
    
    toolbar.setBounds(area.removeFromTop (toolbarHeight));
    if (editorIsVisible) editor.setBounds(area.removeFromBottom(editorHeight));
    if (fileExplorerIsVisible) fileExplorer.setBounds(area.removeFromRight(fileExplorerWidth));
    arranger.setBounds(area);
    buttonRect.setPosition(area.getX(), area.getBottom() - collapseButtonWidth);
    editorCollapser.setBounds(buttonRect);
    
}

MainContentComponent::CollapseButtonComponent::CollapseButtonComponent(float arrowDirection, Colour arrowColour)
    : button("Collapse", arrowDirection, arrowColour)
{
    addAndMakeVisible(button);
}

MainContentComponent::CollapseButtonComponent::CollapseButtonComponent(String buttonName, float arrowDirection, Colour arrowColour)
: button(buttonName, arrowDirection, arrowColour)
{
    addAndMakeVisible(button);
}

void MainContentComponent::CollapseButtonComponent::paint(Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);
}

void MainContentComponent::CollapseButtonComponent::resized()

void MainContentComponent::buttonClicked(Button* button)
{
    
}
