/*
==============================================================================

Network.h
Created: 31 Jan 2018 9:57:16pm
Author:  ValentinoAbate (vabate@ucsc.edu)

==============================================================================
*/


#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

#define PORT 9001
#define TIMEOUT 100 // miliseconds
#define MULTICAST_GROUP "224.0.0.1"
#define MAGIC_NUMBER 0xf2b49e2c

//TODO: change NetworkClient to have a state object or pointer to a state object to manage when to update state

//Utility Functions//---------------------------------------------------------------------------------------------------------------------------------------//

// A few quick utility functions to convert between raw data and ValueTrees
ValueTree memoryBlockToValueTree(const MemoryBlock& mb);
MemoryBlock valueTreeToMemoryBlock(const ValueTree& v);
String valueTreeToString(const ValueTree& v);

//Aplication State Interfaces//-----------------------------------------------------------------------------------------------------------------------------//

//TODO:
//make serialization be sepereate valueTree nodes and make serialization use juce::values wit valuetree::getPropertyAsValue to make everything auto-link

class NetworkClient;
class Transmittable;

//State interface: use an object of this class along with a NetworkClient to transmit state over the network
class State
{
public:
	State(ValueTree state) : state{ state } {}
	//Update underlying state ValueTree from recieved data
	void recieve(const ValueTree& recieved);
	//Build (or update) state ValueTree from current values, and call for transmission
	void transmit(NetworkClient& connection);
	//Update registered objects for current state ValueTree. Should be called when tranmission is recieved
	void updateObjects();
	//Update state ValueTree from objects. should be called before transmission
	void updateState();
	//Adds object to state registry
	bool registerObject(Transmittable& obj);
	//Return XML representation of underlying valueTree
	String toXmlString();
private:
	ValueTree state;
	std::vector<ScopedPointer<Transmittable>> objects = std::vector<ScopedPointer<Transmittable>>();
};
//Transmission interface: have any class implement this to enable transmission for that class
class Transmittable
{
public:
	//Build a Valuetree consisting of all values meant to be transmitted
	virtual void updateTree(ValueTree& t) = 0;
	//Set transmittable values from given ValueTree
	virtual void updateValues(ValueTree& t) = 0;
	//Base ID
	Identifier id = nullptr;
};

//Network Classes//----------------------------------------------------------------------------------------------------------------------------------------//

//Client side of the connection. also created server-side to handle incoming connections
class NetworkClient : public InterprocessConnection
{
public:
	NetworkClient(State& state, uint32 magicMessageHeaderNumber = MAGIC_NUMBER)
		: InterprocessConnection(/*callbacksOnMessageThread*/ false, magicMessageHeaderNumber), state{ state } {};
	virtual void connectionMade() override;
	virtual void connectionLost() override;
	virtual void messageReceived(const MemoryBlock &message) override;
	void printInfo(std::ostream& out = std::cout);
private:
	State& state;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NetworkClient)
};


//Handles incoming connections and sets up TCP connection
class NetworkServer : public InterprocessConnectionServer
{
public:
	NetworkServer();
	//Create new connection on server when new user requests connection
	virtual InterprocessConnection* createConnectionObject();
	void broadcast();
	void printInfo(std::ostream& out = std::cout) const;
private:
	//Current state
	ValueTree state = ValueTree("server_module");
	friend class ServerSideConnection;
	//Special version of NetworkClient for server-side stuff (Fix access)
	class ServerSideConnection : public InterprocessConnection
	{
	public:
		ServerSideConnection(const ValueTree& tree, NetworkServer& parent, uint32 magicMessageHeaderNumber = MAGIC_NUMBER)
			: InterprocessConnection(false, magicMessageHeaderNumber), state{ tree }, server{ parent } {};
		virtual void connectionMade() override;
		virtual void connectionLost() override;
		virtual void messageReceived(const MemoryBlock &message) override;
	private:
		NetworkServer& server;
		ValueTree state;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ServerSideConnection)
	};
	//BroadCast to all but input ServerSideConnection(for broadcasting after 1 client updates the state)
	void broadcast(const ServerSideConnection* ignore) const;
	//Multiple Clients are supported
	OwnedArray<ServerSideConnection> clients = OwnedArray<ServerSideConnection>();
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NetworkServer)
};

//void scanNetwork(NetworkClient* client, NetworkServer* server);

#endif  // NETWORK_H_INCLUDED

