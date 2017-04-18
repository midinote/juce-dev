/*
  ==============================================================================

    Network.cpp
    Created: 15 Apr 2017 10:23:29pm
    Author:  jimi

  ==============================================================================
*/

#include "Network.h"

NetworkClient::NetworkClient(void (*connectionMadePointer) (), void (*connectionLostPointer) (),
                             void (*messageReceivedPointer) (), uint32 magicMessageHeaderNumber)
    : InterprocessConnection(/*callbacksOnMessageThread*/ true, magicMessageHeaderNumber)
{
    this->connectionMadePointer = connectionMadePointer;
    this->connectionLostPointer = connectionLostPointer;
    this->messageReceivedPointer = messageReceivedPointer;
}

void NetworkClient::connectionMade() {
    connectionMadePointer();
}

void NetworkClient::connectionLost() {
    connectionLostPointer();
}

void NetworkClient::messageReceived (const MemoryBlock &message) {
    messageReceivedPointer();
}

NetworkServer::NetworkServer (NetworkClient* client) : InterprocessConnectionServer() {
    this->client = client;
}

InterprocessConnection* NetworkServer::createConnectionObject() {
    // just support one client at a time for now
    if (client->isConnected()) {
        return nullptr;
    }
    return client;
}
