/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "../../NetworkTest/Builds/VisualStudio2017/Network.h"

constexpr bool SHOW_IPV6 = false;

//==============================================================================
int main (int argc, char* argv[])
{
	using namespace std;
    // ..your code goes here!
	Array<IPAddress> addresses = Array<IPAddress>();
	ScopedPointer<NetworkServer> server = ScopedPointer<NetworkServer>(new NetworkServer());
	//NetworkClient* client = new NetworkClient();
	cout << "server is waiting... " << server->beginWaitingForSocket(PORT) << "\n";
	//Construct sample message
	ValueTree tree = ValueTree("Hello Client!");
	MemoryBlock mess = valueTreeToMemoryBlock(tree);

	cout << "Hello World!\n";

	//Get all machine addresses
	IPAddress::findAllAddresses(addresses, SHOW_IPV6);
	//Display addresses
	for (IPAddress addr : addresses)
	{
		cout << addr.toString() << "\n";
	}
	//Print current port (#defined in Network.h)
	cout << "server_port: " << server->getBoundPort() << "\n";

	//Main input loop
	char cmd = '0';
	while (cmd != 'q')
	{
		cin >> cmd;
		if (cmd == 'i')//Show server info
		{
			server->printInfo();
		}
		else if (cmd == 's')//send test message
		{
			server->broadcast();
		}
	}

	server->stop();

    return 0;
}
