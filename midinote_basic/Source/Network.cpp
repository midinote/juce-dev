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
    return client;
}

// returns 0 on success, 1 on failure to find anyone to connect to,
// and 2 on failure to connect to someone found in multicast group
int scanNetwork (NetworkClient* client) {
    Array<IPAddress> interfaceIPs;
    IPAddress::findAllAddresses (interfaceIPs);
    DatagramSocket scanners[interfaceIPs.size()];
    size_t i = 0;
    for (auto scanner : scanners) {
        scanner->bindToPort (PORT, interfaceIPs[i].toString());
        scanner->joinMulticast (MULTICAST_GROUP);
        ++i;
    }
    uint32 buffer = MAGIC_NUMBER;
    // fork this part for each scanner
    for (auto scanner : scanners ) {
        // multicast our message
        scanner->waitUntilReady (false, TIMEOUT); // false means wait to write
        scanner->write (MULTICAST_GROUP, PORT, buffer, sizeof buffer);
        // hopefully receive someone else's message
        scanner->waitUntilReady (true, TIMEOUT); // true means wait to read
        uint32 recv_buffer = 0;
        String senderIPAddress = "";
        int senderPortNumber = 0;
        scanner->read (&recv_buffer, sizeof buffer, false, senderIPAddress, senderPortNumber);
        // check to make sure it was the right message and not our own
        if (recv_buffer == MAGIC_NUMBER and senderPortNumber == PORT
            and !(interfaceIPs.contains(IPAddress (senderIPAddress)))) {
            if (client->connectToSocket (senderIPAddress, PORT, TIMEOUT))
                return 0; // or whatever keyword exits a fork
            else return 2;
        }
    }
    return 1;
}
