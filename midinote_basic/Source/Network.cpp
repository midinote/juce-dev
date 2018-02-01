/*
==============================================================================

Network.cpp
Created: 31 Jan 2018 9:57:16pm
Author:  Valentino Abate (vabate@ucsc.edu)

==============================================================================
*/


#include "Network.h"

// A few quick utility functions to convert between raw data and ValueTrees
ValueTree memoryBlockToValueTree(const MemoryBlock& mb)
{
	return ValueTree::readFromData(mb.getData(), mb.getSize());
}

MemoryBlock valueTreeToMemoryBlock(const ValueTree& v)
{
	MemoryOutputStream mo;
	v.writeToStream(mo);
	return mo.getMemoryBlock();
}

String valueTreeToString(const ValueTree& v)
{
	const ScopedPointer<XmlElement> xml(v.createXml());
	return xml != nullptr ? xml->createDocument("", true, false) : String();
}

void State::recieve(const ValueTree & recieved)
{
	state.copyPropertiesFrom(recieved, nullptr);
}

void State::transmit(NetworkClient& connection)
{
	updateState();
	connection.sendMessage(valueTreeToMemoryBlock(state));
}

void State::updateObjects()
{
	for (const auto& obj : objects)
	{
		obj->updateValues(state);
	}
}

void State::updateState()
{
	for (const auto& obj : objects)
	{
		obj->updateTree(state);
	}
}

bool State::registerObject(Transmittable& obj)
{
	if (state.hasProperty(obj.id))
		return false;
	obj.updateTree(state);
	objects.push_back(&obj);
	return false;
}

String State::toXmlString()
{
	return state.toXmlString();
}

void NetworkClient::connectionMade()
{
	std::cout << "connection made!\n";
}

void NetworkClient::connectionLost()
{
	std::cout << "connection lost :(\n";
}

void NetworkClient::messageReceived(const MemoryBlock& message)
{
	state.recieve(memoryBlockToValueTree(message));
	state.updateObjects();
	std::cout << "message recieved: " << state.toXmlString() << "\n";
}

void NetworkClient::printInfo(std::ostream & out)
{
	out << std::boolalpha;
	out << "connected_to_server: " << isConnected() << std::endl;
	out << "server_host: " << getConnectedHostName() << std::endl;;
	out << "state: " << state.toXmlString() << std::endl;;
}

void NetworkServer::ServerSideConnection::connectionMade()
{
	std::cout << "connection made!\n";
}

void NetworkServer::ServerSideConnection::connectionLost()
{
	std::cout << "connection lost :(\n";
	server.clients.remove(server.clients.indexOf(this));
}

void NetworkServer::ServerSideConnection::messageReceived(const MemoryBlock & message)
{
	state.copyPropertiesFrom(memoryBlockToValueTree(message), nullptr);
	server.broadcast(this);
	std::cout << "message recieved: " << state.toXmlString() << "\n";
}

NetworkServer::NetworkServer() : InterprocessConnectionServer() {

}

InterprocessConnection* NetworkServer::createConnectionObject() {
	std::cout << "client has attempted to connect\n";
	return clients.add(new ServerSideConnection(state, *this));
}

void NetworkServer::broadcast()
{
	const MemoryBlock message = valueTreeToMemoryBlock(state);
	for (const auto& client : NetworkServer::clients)
	{
		client->sendMessage(message);
	}
	std::cout << "server_send: " << state.toXmlString() << std::endl;
}

void NetworkServer::printInfo(std::ostream& out) const
{
	out << std::boolalpha;
	for (const auto& client : clients)
	{
		out << "client_is_connected: " << client->isConnected() << std::endl;
		out << "client_host: " << client->getConnectedHostName() << std::endl;
	}
	out << "server state:\n" << state.toXmlString();
}

void NetworkServer::broadcast(const ServerSideConnection*  ignore) const
{
	const MemoryBlock message = valueTreeToMemoryBlock(state);
	for (const auto& client : NetworkServer::clients)
	{
		if (client != ignore)
			client->sendMessage(message);
	}
	std::cout << "server_send: " << state.toXmlString() << std::endl;
}


