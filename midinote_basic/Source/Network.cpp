/*
  ==============================================================================

    Network.cpp
    Created: 15 Apr 2017 10:23:29pm
    Author:  midinote

  ==============================================================================
*/

#include "Network.h"

NetworkServer::NetworkServer (NetworkClient* client) : InterprocessConnectionServer() {
    this->client = client;
}

InterprocessConnection* NetworkServer::createConnectionObject() {
    // just support one client at a time for now
    //if (client->isConnected()) {
    //    return nullptr;
    //}
    return client;
}

void scanNetwork (NetworkClient* client, NetworkServer* server) {
    Array<IPAddress> interfaceIPs;
    IPAddress::findAllAddresses (interfaceIPs);
    String test = interfaceIPs.contains(IPAddress(HOST_IP)) ? CLIENT_IP : HOST_IP;
    if (test == CLIENT_IP) {
        server->beginWaitingForSocket (PORT, ""); // empty string means listen on all host IPs
        std::cout << "being a host!" << std::endl;
    }
    if (client->connectToSocket (test, PORT, TIMEOUT)) {
        //server->stop();
        std::cout << "Connected to " << test << std::endl;
    }
    /*
    // try to connect to all possible IPv4 addresses
    server->beginWaitingForSocket (PORT, ""); // empty string means listen on all host IPs
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
    */
    // note that server->stop() is only called if the client connects to someone else
}
