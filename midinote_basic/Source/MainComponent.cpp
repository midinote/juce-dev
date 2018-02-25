#include "MainComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

MenuComponent::MenuComponent()
:   IPbox ("IPbox"),
    IPbutton ("Connect",
    /* tooltip */ "Connect to the specified IP address, or leave blank for localhost.")
{
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setColour (Label::textColourId, Colours::white);
    midiInputListLabel.setText ("Midi Input: ", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);

    addAndMakeVisible (presetsListLabel);
    presetsListLabel.setColour (Label::textColourId, Colours::white);
    presetsListLabel.setText ("Preset: ", dontSendNotification);
    presetsListLabel.attachToComponent (&presetsList, true);

    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No Devices Available");

    addAndMakeVisible (presetsList);
    presetsList.setTextWhenNoChoicesAvailable ("None");

    addAndMakeVisible (IPbox);
    addAndMakeVisible (IPbutton);
    IPbox.setTextToShowWhenEmpty ("IP Address", Colours::grey);
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
    midiInputList.setBounds (menuItemsSpace, 10, 250, headerMenuHeight - 16);
    menuItemsSpace += midiInputList.getRight();
    presetsList.setBounds (menuItemsSpace, 10, 180, headerMenuHeight - 16);
    menuItemsSpace += presetsList.getRight();
    menuItemsSpace -= 380; // VERY weird bug
    IPbox.setBounds (menuItemsSpace, 10, 110, headerMenuHeight - 16);
    menuItemsSpace += IPbox.getRight();
    menuItemsSpace -= 630; // VERY weird bug
    IPbutton.setBounds (menuItemsSpace, 10, 80, headerMenuHeight - 16);
    menuItemsSpace += IPbutton.getRight();
}

MainContentComponent::MainContentComponent()
:   lastInputIndex (0),
    isAddingFromMidiInput (false),
    noteOn (false),
	globalState(ValueTree("global_state")),
	connection(globalState)
{
	osc1.setID("osc1");
	globalState.registerObject(osc1);
	osc2.setID("osc2");
	globalState.registerObject(osc2);
    addAndMakeVisible(headerMenu);

    addAndMakeVisible(midiEditor);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);

    const StringArray midiInputs (MidiInput::getDevices());
    headerMenu.midiInputList.addItemList(midiInputs, 1);
    headerMenu.midiInputList.addListener (this);
    headerMenu.presetsList.addListener (this);
    headerMenu.IPbox.addListener (this);
    headerMenu.IPbutton.addListener (this);
    osc1.envelopeMenu.addListener (this);
    osc2.envelopeMenu.addListener (this);

    midiEditor.keyboardState.addListener (this);
    osc1.panSlider.addListener (this);
    osc1.frequencySlider.addListener (this);
    osc1.levelSlider.addListener (this);
    osc1.waveButtons.addListenerToButtons (this);
    osc1.filter.addListenerToButtons (this);
    osc1.cutoffSlider.addListener (this);
    osc1.resonanceSlider.addListener (this);
    osc1.attackSlider.addListener (this);
    osc1.decaySlider.addListener (this);
    osc1.sustainSlider.addListener (this);
    osc1.releaseSlider.addListener (this);

    osc2.panSlider.addListener (this);
    osc2.frequencySlider.addListener (this);
    osc2.levelSlider.addListener (this);
    osc2.waveButtons.addListenerToButtons (this);
    osc2.filter.addListenerToButtons (this);
    osc2.cutoffSlider.addListener (this);
    osc2.resonanceSlider.addListener (this);
    osc2.attackSlider.addListener (this);
    osc2.decaySlider.addListener (this);
    osc2.sustainSlider.addListener (this);
    osc2.releaseSlider.addListener (this);


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

void MainContentComponent::networkConnect (std::string address)
{
	//Default Address to use (127.0.0.1 for localhost)
	if (address == "") address = "127.0.0.1";
	std::cout << "attempt_connection to " << address << ": " << connection.connectToSocket(address, PORT, TIMEOUT) << "\n";
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
    Filter* filter = &(osc1.filter);
    // the way JUCE's IIRFilter library is designed, it has its own off-switch (makeInactive())
    switch (filter->getMode()) {
        case Filter::low:
            filter->left.setCoefficients(IIRCoefficients::makeLowPass (currentSampleRate,
                                         filter->cutoff, filter->resonance));
            filter->right.setCoefficients(IIRCoefficients::makeLowPass (currentSampleRate,
                                          filter->cutoff, filter->resonance));
            break;
        case Filter::high:
            filter->left.setCoefficients(IIRCoefficients::makeHighPass (currentSampleRate,
                                         filter->cutoff, filter->resonance));
            filter->right.setCoefficients(IIRCoefficients::makeHighPass (currentSampleRate,
                                          filter->cutoff, filter->resonance));
            break;
        case Filter::band:
            filter->left.setCoefficients(IIRCoefficients::makeBandPass (currentSampleRate,
                                         filter->cutoff, filter->resonance));
            filter->right.setCoefficients(IIRCoefficients::makeBandPass (currentSampleRate,
                                          filter->cutoff, filter->resonance));
            break;
        default:
            filter->left.makeInactive();
            filter->right.makeInactive();
            break;
    }
    filter->left.processSamples (bufferL, bufferSize);
    filter->right.processSamples (bufferR, bufferSize);
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
	globalState.updateState();
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
    // conveniently, none (yet) of our ComboBox objects are something that should be sent over the network
}

void MainContentComponent::buttonClicked (Button* button)
{
    if (button == &(headerMenu.IPbutton)) {
        if (headerMenu.IPbox.isEmpty()
        or headerMenu.IPbox.getText().equalsIgnoreCase ("localhost")
        or headerMenu.IPbox.getText().equalsIgnoreCase ("local"))
            networkConnect(); // localhost
        else // if some sort of check that it is a valid IP address
            networkConnect (headerMenu.IPbox.getText().toStdString());
    } else if (osc1.waveButtons.contains (button)
            or osc2.waveButtons.contains (button)
            or osc1.filter.contains (button)
            or osc2.filter.contains (button)) {
        globalState.updateState();
    }
}

void MainContentComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &(osc1.frequencySlider)
     or slider == &(osc1.levelSlider)
     or slider == &(osc1.panSlider)
     or slider == &(osc1.cutoffSlider)
     or slider == &(osc1.resonanceSlider)
     or slider == &(osc1.attackSlider)
     or slider == &(osc1.decaySlider)
     or slider == &(osc1.sustainSlider)
     or slider == &(osc1.releaseSlider)
     or slider == &(osc2.frequencySlider)
     or slider == &(osc2.levelSlider)
     or slider == &(osc2.panSlider)
     or slider == &(osc2.cutoffSlider)
     or slider == &(osc2.resonanceSlider)
     or slider == &(osc2.attackSlider)
     or slider == &(osc2.decaySlider)
     or slider == &(osc2.sustainSlider)
     or slider == &(osc2.releaseSlider))
        globalState.updateState();
}

void MainContentComponent::textEditorReturnKeyPressed (TextEditor& box)
{
    if (box.getName().compare ("IPbox") == 0)
        buttonClicked (&(headerMenu.IPbutton));
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
