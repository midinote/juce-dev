/*
  ==============================================================================

    Network.h
    Created: 15 Apr 2017 10:23:29pm
    Author:  midinote

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
    NetworkClient (uint32 magicMessageHeaderNumber = MAGIC_NUMBER)
    : InterprocessConnection(/*callbacksOnMessageThread*/ true, magicMessageHeaderNumber);
    virtual void connectionMade()=0;
    virtual void connectionLost()=0;
    virtual void messageReceived (const MemoryBlock &message)=0;
};

class NetworkServer : public InterprocessConnectionServer
{
public:
    // just support one client at a time for now
    NetworkClient* client;
    NetworkServer (NetworkClient* client);
    virtual InterprocessConnection* createConnectionObject();
};

void scanNetwork (NetworkClient* client, NetworkServer* server);

#endif  // NETWORK_H_INCLUDED
