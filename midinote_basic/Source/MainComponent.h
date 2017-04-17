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

class MainContentComponent   : public AudioAppComponent,
                               private MidiInputCallback,
                               private ComboBoxListener,
                               private MidiKeyboardStateListener
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
    
private:
    MidiEditor midiEditor;
    Synth synth;
    MidiMessage currentNote;
    ComboBox midiInputList;
    Label midiInputListLabel;
    
    double currentSampleRate;
    int lastInputIndex;
    bool isAddingFromMidiInput;
    bool noteOn;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
