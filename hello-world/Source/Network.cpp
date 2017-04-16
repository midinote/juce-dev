/*
  ==============================================================================

    Network.cpp
    Created: 15 Apr 2017 10:23:29pm
    Author:  jimi

  ==============================================================================
*/

#include "Network.h"

void NetworkServer::setClient (NetworkClient* newClient) {
    this->client = newClient;
}

InterprocessConnection* NetworkServer::createConnectionObject() {
    // just support one client at a time for now
    if (client->isConnected()) {
        this->stop();
        return nullptr;
    }
    return client;
}

void NetworkClient::connectionMade() {
    // we could just do this with InterprocessConnection::getConnectedHostName(), but I want
    // to get the hang of MemoryBlock and really see these messages truly go back and forth
    String hostname = SystemStats::getComputerName();
    this->sendMessage (MemoryBlock (&hostname, sizeof (hostname)));
}

void NetworkClient::messageReceived (const MemoryBlock &message) {
    char hostname[256]; // 255 is the max length of a computer's hostname in POSIX
    message.copyTo (hostname, 0, message.getSize());
    this->helloMessage = hostname;
}
