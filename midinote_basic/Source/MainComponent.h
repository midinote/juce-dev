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
#include "../JuceLibraryCode/JuceHeader.h"

#define headerMenuHeight 36
#define headerMenuMargin 4

class MenuComponent         : public Component
{
public:
    ComboBox midiInputList;
    Label midiInputListLabel;
    ComboBox presetsList;
    Label presetsListLabel;

    MenuComponent();
    ~MenuComponent();

    void paint (Graphics& g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};

class MainContentComponent   : public AudioAppComponent,
                               private MidiInputCallback,
                               private ComboBoxListener,
                               private MidiKeyboardStateListener,
                               private SliderListener
{
public:
    MainContentComponent();
    ~MainContentComponent();

    void prepareToPlay(int, double) override;
    void getNextAudioBlock(const AudioSourceChannelInfo&) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;

    void handleNoteOn(MidiKeyboardState*, int, int, float) override;
    void handleNoteOff(MidiKeyboardState*, int, int, float) override;

    void handleIncomingMidiMessage (MidiInput*, const MidiMessage&) override;

    void comboBoxChanged (ComboBox*) override;

    void setMidiInput (int);

    void sliderValueChanged(Slider*) override;

private:
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
