#include "MainComponent.h"
#include "Network.h"

MainContentComponent::MainContentComponent()
{
    setSize(1280, 714);

    addAndMakeVisible(synth);

    networkServer = new NetworkServer (this);
    scanNetwork (this, networkServer);
    
    

}

MainContentComponent::~MainContentComponent()
{
    delete networkServer;
    sharedAudioDeviceManager = nullptr;
    delete this;
}

void MainContentComponent::paint(Graphics& g)
{
}

void MainContentComponent::resized()
{
    Rectangle<int> area = getLocalBounds();
    synth.setBounds(0, 0, area.getWidth(), area.getHeight());
}

void MainContentComponent::connectionMade()
{
    SynthComponent::Settings* settings = synth.getSettings();
    sendMessage (MemoryBlock (settings, sizeof(settings)));
}

void MainContentComponent::connectionLost()
{
    String message = "Connection lost";
}

void MainContentComponent::messageReceived(const MemoryBlock& message)
{
    SynthComponent::Settings newSettings = *static_cast<SynthComponent::Settings*>(message.getData());
    synth.updateSettings(newSettings);
}

void MainContentComponent::sliderValueChanged(Slider* slider)
{
    SynthComponent::Settings* settings = synth.getSettings();
    sendMessage (MemoryBlock (settings, sizeof(settings)));
}
