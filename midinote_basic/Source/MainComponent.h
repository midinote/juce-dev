/*
  ==============================================================================

    MainComponent.h
    Created: 7 Apr 2017 6:29:04pm
    Author:  Malcolm Flint

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED
#include "MidiEditor.h"
#include "Synth.h"
#include "Network.h"
#include "../JuceLibraryCode/JuceHeader.h"

#ifdef _WIN32
#define and &&
#define or ||
#define not !
#endif

#define headerMenuHeight 36
#define headerMenuMargin 4

class MenuComponent          : public Component
{
public:
    ComboBox midiInputList;
    Label midiInputListLabel;
    ComboBox presetsList;
    Label presetsListLabel;
    TextEditor IPbox;
    TextButton IPbutton;

    MenuComponent();
    ~MenuComponent();

    void paint (Graphics& g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};

class MainContentComponent   : public AudioAppComponent,
                               public TextEditor::Listener,
                               public Button::Listener,
                               public ComboBox::Listener,
                               public Slider::Listener,
                               private MidiInputCallback,
                               private MidiKeyboardStateListener
{
public:
    MainContentComponent();
    ~MainContentComponent();

    void networkConnect (std::string address = ""); // defaults to localhost

    void prepareToPlay (int, double) override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;

    void handleNoteOn (MidiKeyboardState*, int, int, float) override;
    void handleNoteOff (MidiKeyboardState*, int, int, float) override;
    void focusLost (FocusChangeType) override;
    void focusOfChildComponentChanged (FocusChangeType) override;

    void handleIncomingMidiMessage (MidiInput*, const MidiMessage&) override;

    void comboBoxChanged (ComboBox*) override;
    void buttonClicked (Button* button) override;
    void textEditorReturnKeyPressed (TextEditor&) override;
    void sliderValueChanged (Slider*) override;

    void setMidiInput (int);

private:
	State globalState;
	NetworkClient connection;
    MenuComponent headerMenu;
    MidiEditor midiEditor;
    Synth osc1;
    Synth osc2;
    MidiMessage currentNote;

    double currentSampleRate;
    int lastInputIndex;
    bool isAddingFromMidiInput;
    bool noteOn;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
