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
        client = new NetworkClient(this, &helloMessage);
        server = new NetworkServer(client);

        scanNetwork();
    }

    ~MainContentComponent()
    {
        client->disconnect();
        server->stop();
        delete client;
        delete server;
        shutdownAudio();
    }

    //==============================================================================

    // Scans the network for possible connections and attempts to connect to them.
    // If none are found, the server thread is left constantly scanning for any new clients
    void scanNetwork() {
        server->beginWaitingForSocket (PORT, ""); // empty string means listen on all host IPs
        // try to connect to all possible IPv4 addresses
        Array<IPAddress> interfaceIPs;
        IPAddress::findAllAddresses (interfaceIPs);
        for (auto ip : interfaceIPs) {
            //if (ip.address[0] == 127) continue; // skip localhost
            for (uint8 i = 2; i <= 255; ++i) {
                IPAddress test (ip.address[0], ip.address[1], ip.address[2], i);
                if (ip == test) continue; // avoid trying to connect to ourself
                if (client->isConnected()
                || client->connectToSocket (test.toString(), PORT, TIMEOUT)) {
                    // found a connection, so stop looking for more
                    std::cout << "Connected to " << test.toString() << std::endl;
                    server->stop();
                    return;
                }
            }
        }
        // note that server->stop() is only called if the client connects to someone else
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
