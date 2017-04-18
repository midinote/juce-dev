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
    void (*connectionMadePointer)();
    void (*connectionLostPointer)();
    void (*messageReceivedPointer)(const MemoryBlock&);
    NetworkClient (void (*connectionMadePointer)(), void (*connectionLostPointer)(),
                   void (*messageReceivedPointer)(const MemoryBlock&),
                   uint32 magicMessageHeaderNumber = MAGIC_NUMBER);
    virtual void connectionMade();
    virtual void connectionLost();
    virtual void messageReceived (const MemoryBlock &message);
};

class NetworkServer : public InterprocessConnectionServer
{
public:
    // just support one client at a time for now
    NetworkClient* client;
    NetworkServer (NetworkClient* client);
    virtual InterprocessConnection* createConnectionObject();
};

// Scans the network for possible connections and attempts to connect to
// them. If none are found, the server thread is left constantly
// scanning for any new clients.
void scanNetwork (NetworkClient* client, NetworkServer* server);

#endif  // NETWORK_H_INCLUDED
