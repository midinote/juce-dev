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
    ImageButton editorCollapser;
    FileExplorer fileExplorer;
    ImageButton fileExplorerCollapser;
    
    bool editorIsVisible, fileExplorerIsVisible;
    const int toolbarHeight;
    int editorHeight, editorCollapserHeight, editorCollapserWidth,
        fileExplorerWidth, fileExplorerCollapserHeight, fileExplorerCollapserWidth;
    
    Image collapseButtonImageUp, collapseButtonImageDown, collapseButtonImageLeft, collapseButtonImageRight;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
