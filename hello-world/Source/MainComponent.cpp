/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Network.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:

    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);

        // initialize network stuff
        helloMessage = "";
        client = new NetworkClient(&MainContentComponent::connectionMade,
                                   &MainContentComponent::connectionLost,
                                   &MainContentComponent::messageReceived, MAGIC_NUMBER);
        server = new NetworkServer(client);
        scanNetwork(client, server);
    }

    ~MainContentComponent()
    {
        delete client;
        delete server;
        shutdownAudio();
    }

    //==============================================================================

    void connectionMade() {
        // we could just do this with InterprocessConnection::getConnectedHostName(), but I want
        // to get the hang of MemoryBlock and really see these messages truly go back and forth
        String hostname = SystemStats::getComputerName();
        client->sendMessage (MemoryBlock (&hostname, sizeof (hostname)));
    }

    void connectionLost() {
        helloMessage = "Connection lost";
    }

    void messageReceived (const MemoryBlock &message) {
        char hostname[256]; // 255 is the max length of a computer's hostname in POSIX
        message.copyTo (hostname, 0, message.getSize());
        String temp = "Hello World, from ";
        temp += hostname;
        helloMessage = temp;
        repaint();
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);


        // You can add your drawing code here!
        if (helloMessage != "") {
            g.setColour (Colours::white);
            g.setFont (24.0f);
            g.drawText (helloMessage, getLocalBounds(), Justification::centred);
        }
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================

    // Your private member variables go here...
    NetworkClient* client;
    NetworkServer* server;
    String helloMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
