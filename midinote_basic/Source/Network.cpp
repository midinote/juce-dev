/*
==============================================================================

Network.cpp
Created: 31 Jan 2018 9:57:16pm
Author:  Valentino Abate (vabate@ucsc.edu)

==============================================================================
*/


#include "Network.h"

// UTILITY FUNCTIONS//

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

MemoryBlock propertyToMemoryBlock(const var& val, const Identifier& identifier)
{
	ValueTree v("message");
	v.removeAllProperties(nullptr);
	v.setProperty(identifier, val, nullptr);
	return valueTreeToMemoryBlock(v);
}

//STATE CODE//

void State::recieve(const ValueTree & recieved)
{
	Identifier i = recieved.getPropertyName(0);
	state.setPropertyExcludingListener(&listener, i, recieved.getProperty(i), nullptr);
//	objects.at(i.toString().toStdString())->updateValues(state);
}

void State::updateObjects()
{
	for (const auto& obj : objects)
	{
		obj.second->updateValues(*this);
	}
}

void State::updateState()
{
	for (const auto& obj : objects)
	{
		obj.second->updateTree(*this, true);
	}
}

bool State::registerObject(Transmittable& obj)
{
	if (state.hasProperty(obj.id))
		return false;
	obj.updateTree(*this, false);
	objects.insert({ obj.id.toString().toStdString(), &obj });
	return true;
}

String State::toXmlString()
{
	return state.toXmlString();
}

void State::transmit(const var & val, const Identifier & identifier)
{
	connection->sendMessage(propertyToMemoryBlock(val, identifier));
}

//STATE::LISTENER CODE//

void State::Listener::valueTreePropertyChanged(ValueTree & treeWhosePropertyHasChanged, const Identifier & property)
{
	parent.transmit(treeWhosePropertyHasChanged.getProperty(property), property);
}

void State::Listener::valueTreeChildAdded(ValueTree & parentTree, ValueTree & childWhichHasBeenAdded)
{
}

void State::Listener::valueTreeChildRemoved(ValueTree & parentTree, ValueTree & childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
}

void State::Listener::valueTreeChildOrderChanged(ValueTree & parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
}

void State::Listener::valueTreeParentChanged(ValueTree & treeWhoseParentHasChanged)
{
}

//NETWORK CLIENT CODE//

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

//NETWORK SERVER CODE//

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
	out << "server state:\n" << valueTreeToString(state);
}

void NetworkServer::broadcast(const ServerSideConnection*  ignore, const Identifier& id) const
{
	const MemoryBlock message = propertyToMemoryBlock(state.getProperty(id), id);
	for (const auto& client : NetworkServer::clients)
	{
		if (client != ignore)
			client->sendMessage(message);
	}
	std::cout << "server_send: " << valueTreeToString(state) << std::endl;
}

//NetworkServer::ServerSideConnection code//

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
	ValueTree v = memoryBlockToValueTree(message);
	Identifier i = v.getPropertyName(0);
	state.setProperty(i, v.getProperty(i), nullptr);
	server.broadcast(this, i);
	std::cout << "message recieved: " << valueTreeToString(v) << std::endl;
}
