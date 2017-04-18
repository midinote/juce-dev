/*
  ==============================================================================

    Network.cpp
    Created: 15 Apr 2017 10:23:29pm
    Author:  jimi

  ==============================================================================
*/

#include "Network.h"

NetworkClient::NetworkClient (void (*connectionMadePointer)(), void (*connectionLostPointer)(),
               void (*messageReceivedPointer)(const MemoryBlock&),
               uint32 magicMessageHeaderNumber)
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
    messageReceivedPointer (message);
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

void scanNetwork (NetworkClient* client, NetworkServer* server) {
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
