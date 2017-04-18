/*
  ==============================================================================

    Network.h
    Created: 15 Apr 2017 10:23:29pm
    Author:  jimi

  ==============================================================================
*/

#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#define PORT 9001
#define TIMEOUT 100 // miliseconds
#define MAGIC_NUMBER 0xf2b49e2c

class NetworkClient : public InterprocessConnection
{
public:
    NetworkClient (void (*connectionMadePointer) (), void (*connectionLostPointer) (),
                   void (*messageReceivedPointer) (), uint32 magicMessageHeaderNumber = MAGIC_NUMBER);
    virtual void connectionMade()=0;
    virtual void connectionLost()=0;
    virtual void messageReceived (const MemoryBlock &message)=0;
};

class NetworkServer : public InterprocessConnectionServer
{
public:
    // just support one client at a time for now
    NetworkServer (NetworkClient* client);
    virtual InterprocessConnection* createConnectionObject()=0;
};

#endif  // NETWORK_H_INCLUDED
