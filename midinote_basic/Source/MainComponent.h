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

class MainContentComponent   : public Component,
                               public NetworkClient,
                               private SliderListener
{
public:
    MainContentComponent();
    ~MainContentComponent();

    static AudioDeviceManager& getSharedAudioDeviceManager();
    
    void paint (Graphics& g) override;
    void resized() override;

private:
    static void runtimePermissionsCallback (bool wasGranted);

    SynthComponent synth;
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const MemoryBlock& message) override;
    NetworkServer* networkServer;
    
    void sliderValueChanged(Slider* slider) override;

    String helloMessage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
