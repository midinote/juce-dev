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

    void paint (Graphics& g) override;
    void resized() override;

private:
    class CollapseButtonComponent  : public Button
    {
    public:
        CollapseButtonComponent(float arrowDirection, Colour arrowColour);
        CollapseButtonComponent(String buttonName, float arrowDirection, Colour arrowColour);
        ~CollapseButtonComponent();
        void paint (Graphics& g) override;
        void resized() override;
    private:
        ArrowButton button;
    };
    
    void buttonClicked(Button* button) override;
    
    ToolBar toolbar;
    Arranger arranger;
    Editor editor;
    ArrowButton editorCollapser;
    FileExplorer fileExplorer;
    ArrowButton fileExplorerCollapser;
    
    bool editorIsVisible, fileExplorerIsVisible;
    const int toolbarHeight;
    int editorHeight, fileExplorerWidth, collapseButtonWidth;
    
    Image collapseButtonImageUp, collapseButtonImageDown, collapseButtonImageLeft, collapseButtonImageRight;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
