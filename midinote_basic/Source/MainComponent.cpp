#include "MainComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

MenuComponent::MenuComponent()
{
    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setColour(Label::textColourId, Colours::white);
    midiInputListLabel.setText("Midi Input: ", dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    addAndMakeVisible(presetsListLabel);
    presetsListLabel.setColour(Label::textColourId, Colours::white);
    presetsListLabel.setText("Preset: ", dontSendNotification);
    presetsListLabel.attachToComponent(&presetsList, true);

    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No Devices Available");

    addAndMakeVisible(presetsList);
    presetsList.setTextWhenNoChoicesAvailable("None");
}

MenuComponent::~MenuComponent()
{
}

void MenuComponent::paint (Graphics& g)
{
    g.fillAll (Colour (55, 56, 55));
}

void MenuComponent::resized()
{
    int menuItemsSpace = 0;
    menuItemsSpace += 85; // length of "Midi Input: " text
    midiInputList.setBounds(menuItemsSpace, 10, 250, headerMenuHeight - 16);
    menuItemsSpace += midiInputList.getRight();
    presetsList.setBounds(menuItemsSpace, 10, 180, headerMenuHeight - 16);
    menuItemsSpace += presetsList.getRight();
}

MainContentComponent::MainContentComponent()
:   lastInputIndex (0),
    isAddingFromMidiInput (false),
    noteOn(false)
{
    addAndMakeVisible(headerMenu);

    addAndMakeVisible(midiEditor);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);

    const StringArray midiInputs (MidiInput::getDevices());
    headerMenu.midiInputList.addItemList(midiInputs, 1);
    headerMenu.midiInputList.addListener(this);
    headerMenu.presetsList.addListener(this);
    osc1.envelopeMenu.addListener(this);
    osc2.envelopeMenu.addListener(this);

    midiEditor.keyboardState.addListener(this);
    osc1.panSlider.addListener(this);
    osc1.frequencySlider.addListener(this);
    osc1.levelSlider.addListener(this);
    osc1.waveSlider.addListener(this);
    osc2.panSlider.addListener(this);
    osc2.frequencySlider.addListener(this);
    osc2.levelSlider.addListener(this);
    osc2.waveSlider.addListener(this);


    for (int i; i < midiInputs.size(); ++i)
    {
        if (deviceManager.isMidiInputEnabled(midiInputs[i]))
        {
            setMidiInput (i);
        }
    }
    if (headerMenu.midiInputList.getSelectedId() == 0)
        setMidiInput (0);

    setSize(1024, 576);
    setAudioChannels(2, 2);

}

MainContentComponent::~MainContentComponent()
{
    //any future networking objects should be deleted/freed here
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
        std::pair<float,float> currentSample = osc1.synthesize(currentSampleRate);
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
    int midiEditorHeight = 130 - headerMenuMargin;
    int synthHeight = (area.getHeight() - (midiEditorHeight + headerMenuHeight))
                       / 2 - headerMenuMargin * 2;
    headerMenu.setBounds (0, 0, area.getWidth(), headerMenuHeight);
    midiEditor.setBounds(0, headerMenuHeight + headerMenuMargin * 3 + synthHeight * 2,
                         area.getWidth(), midiEditorHeight);
    osc1.setBounds(0, headerMenuHeight + headerMenuMargin, area.getWidth(), synthHeight);
    osc2.setBounds(0, osc1.getBottom() + headerMenuMargin, area.getWidth(), synthHeight);
}

void MainContentComponent::handleNoteOn(MidiKeyboardState* state,
                                        int midiChannel,
                                        int midiNoteNumber,
                                        float velocity)
{
    osc1.addNote (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
}

void MainContentComponent::handleNoteOff(MidiKeyboardState* state,
                                        int midiChannel,
                                        int midiNoteNumber,
                                        float velocity)
{
    osc1.removeNote(MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
}

void MainContentComponent::handleIncomingMidiMessage (MidiInput*, const MidiMessage& m)
{
    if (m.isNoteOn()) osc1.addNote(m);
    else if (m.isNoteOff()) osc1.removeNote(m);
}

void MainContentComponent::comboBoxChanged (ComboBox* box)
{
    if (box == &(headerMenu.midiInputList)) {
        setMidiInput(headerMenu.midiInputList.getSelectedItemIndex());
    } else if (box == &(headerMenu.presetsList)) {
        // do stuff around setting presets
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
    headerMenu.midiInputList.setSelectedId(ind+1, dontSendNotification);
    lastInputIndex = ind;
}

void MainContentComponent::sliderValueChanged(Slider* slider)
{
}
