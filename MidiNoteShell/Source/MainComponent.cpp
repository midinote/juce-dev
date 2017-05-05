/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
    : editorCollapser("Collapse Editor", DrawableButton::ImageOnButtonBackground),
      fileExplorerCollapser("Collapse File Explorer", DrawableButton::ImageOnButtonBackground),
      toolbarHeight(40),
      editorHeight(200),
      fileExplorerWidth(200),
      collapserButtonHeight(18),
      absoluteSourceFilePath("/Users/malcolmflint/dev/midinote/juce-dev/MidiNoteShell/Source/")
{
    buttonDrawableUp = Drawable::createFromImageFile(File(absoluteSourceFilePath + "images/collapseButton/upButton.png"));
    buttonDrawableDown = Drawable::createFromImageFile(File(absoluteSourceFilePath + "images/collapseButton/downButton.png"));
    buttonDrawableLeft = Drawable::createFromImageFile(File(absoluteSourceFilePath + "images/collapseButton/leftButton.png"));
    buttonDrawableRight = Drawable::createFromImageFile(File(absoluteSourceFilePath + "images/collapseButton/rightButton.png"));
    
    addAndMakeVisible(toolbar);
    
    addAndMakeVisible(arranger);
    
    addChildComponent(editor);
    addAndMakeVisible(editorCollapser);

    editorCollapser.setImages(buttonDrawableUp, nullptr, nullptr, nullptr,
                              buttonDrawableDown, nullptr, nullptr, nullptr);
    editorCollapser.setClickingTogglesState (true);
    editorCollapser.setTooltip(editorCollapser.getName());
    editorCollapser.addListener(this);
    
    addChildComponent(fileExplorer);
    addAndMakeVisible(fileExplorerCollapser);
    fileExplorerCollapser.setImages(buttonDrawableLeft, nullptr, nullptr, nullptr,
                                    buttonDrawableRight, nullptr, nullptr, nullptr);
    fileExplorerCollapser.setClickingTogglesState (true);
    fileExplorerCollapser.setTooltip(fileExplorerCollapser.getName());
    fileExplorerCollapser.addListener(this);
        
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
    if (editor.isVisible()) editor.setBounds(area.removeFromBottom(editorHeight));
    if (fileExplorer.isVisible()) fileExplorer.setBounds(area.removeFromRight(fileExplorerWidth));
    arranger.setBounds(area);
    editorCollapser.setBounds(area.getX(),
                              area.getBottom() - collapserButtonHeight,
                              collapserButtonHeight,
                              collapserButtonHeight);
    fileExplorerCollapser.setBounds(area.getRight() - collapserButtonHeight,
                                    area.getY(),
                                    collapserButtonHeight,
                                    collapserButtonHeight);
}

void MainContentComponent::buttonClicked(Button* button)
{
    if (button == &editorCollapser) editor.setVisible(!editor.isVisible());
    if (button == &fileExplorerCollapser) fileExplorer.setVisible(!fileExplorer.isVisible());
    resized();

}
