/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "ToolBar.h"
#include "Arranger.h"
#include "Editor.h"
#include "FileExplorer.h"

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
                               private ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    
    void showEditor();
    void showFileExplorer();

    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked(Button* button) override;
    
    ToolBar toolbar;
    Arranger arranger;
    Editor editor;
    DrawableButton editorCollapser;
    FileExplorer fileExplorer;
    DrawableButton fileExplorerCollapser;
    
    const int toolbarHeight;
    int editorHeight,fileExplorerWidth, collapserButtonHeight;
    String absoluteSourceFilePath;
    ScopedPointer<Drawable> buttonDrawableUp, buttonDrawableDown, buttonDrawableLeft, buttonDrawableRight;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
