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
#define MAGIC_NUMBER 0xf2b49e2c // handshake number for network stuff; currently default and currently not incorporated into the code

class NetworkClient : public InterprocessConnection
{
public:
    String helloMessage;
    virtual void connectionMade();
    virtual void messageReceived (const MemoryBlock &message);
};

class NetworkServer : public InterprocessConnectionServer
{
public:
    // just support one client at a time for now
    NetworkClient* client;
    void setClient (NetworkClient* newClient);
    virtual InterprocessConnection* createConnectionObject();
};

#endif  // NETWORK_H_INCLUDED
