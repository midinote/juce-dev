#include "MainComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

MainContentComponent::MainContentComponent()
:   lastInputIndex (0),
    isAddingFromMidiInput (false),
    noteOn(false)
{
    setSize(1280, 714);

    addAndMakeVisible(midiEditor);
    addAndMakeVisible(synth);

    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("Midi Input: ", dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No Midi Devices Available");
    const StringArray midiInputs (MidiInput::getDevices());
    midiInputList.addItemList(midiInputs, 1);
    midiInputList.addListener(this);

    midiEditor.keyboardState.addListener(this);
    synth.panSlider.addListener(this);
    synth.frequencySlider.addListener(this);
    synth.levelSlider.addListener(this);
    synth.waveSlider.addListener(this);


    for (int i; i < midiInputs.size(); ++i)
    {
        if (deviceManager.isMidiInputEnabled(midiInputs[i]))
        {
            setMidiInput (i);
        }
    }
    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);

    setAudioChannels(2, 2);

}

MainContentComponent::~MainContentComponent()
{
    //delete networkServer;
    shutdownAudio();
    delete this;
}

void MainContentComponent::prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate)
{
    currentSampleRate = sampleRate;
}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    float* const bufferL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    float* const bufferR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    int bufferSize = bufferToFill.numSamples;
    for (int sample=0; sample < bufferSize; ++sample)
    {
        std::pair<float,float> currentSample = synth.synthesize(currentSampleRate);
        bufferL[sample] = currentSample.first;
        bufferR[sample] = currentSample.second;
    }
}

void MainContentComponent::releaseResources()
{
}

void MainContentComponent::paint(Graphics& g)
{
}

void MainContentComponent::resized()
{
    Rectangle<int> area = getLocalBounds();
    int midiInputListHeight = 36;
    int midiEditorHeight = 130;
    int synthHeight = area.getHeight() - (midiEditorHeight + midiInputListHeight);
    midiInputList.setBounds(100, 10, 250, midiInputListHeight - 16)
    ;    midiEditor.setBounds(0, synthHeight + midiInputListHeight, area.getWidth(), midiEditorHeight);
    synth.setBounds(0, midiInputListHeight, area.getWidth(), synthHeight);
}

void MainContentComponent::handleNoteOn(MidiKeyboardState* state,
                                        int midiChannel,
                                        int midiNoteNumber,
                                        float velocity)
{
    synth.addNote (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
}

void MainContentComponent::handleNoteOff(MidiKeyboardState* state,
                                        int midiChannel,
                                        int midiNoteNumber,
                                        float velocity)
{
    synth.removeNote(MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
}

void MainContentComponent::handleIncomingMidiMessage (MidiInput*, const MidiMessage& m)
{
    if (m.isNoteOn()) synth.addNote(m);
    else if (m.isNoteOff()) synth.removeNote(m);
}

void MainContentComponent::comboBoxChanged (ComboBox* box)
{
    if (box == &midiInputList) {
        setMidiInput(midiInputList.getSelectedItemIndex());
    }
}

void MainContentComponent::setMidiInput (int ind)
{
    const StringArray list (MidiInput::getDevices());
    deviceManager.removeMidiInputCallback(list[lastInputIndex], this);
    const String newMidiInput (list[ind]);
    if (!deviceManager.isMidiInputEnabled(newMidiInput)) {
        deviceManager.setMidiInputEnabled(newMidiInput, true);
    }
    deviceManager.addMidiInputCallback(newMidiInput, this);
    midiInputList.setSelectedId(ind+1, dontSendNotification);
    lastInputIndex = ind;
}

void MainContentComponent::sliderValueChanged(Slider* slider)
{
}
