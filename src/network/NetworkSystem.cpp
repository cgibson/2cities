#include "NetworkSystem.h"

// Method to take a WorldObject* and update/add it to the main vector (based on ID field)
// ** NOTE: Current VERY inefficient **
// TODO Improve efficiency
void NetworkSystem::updateLocalObject(WorldObject *obj) {
	int i=0;

	std::vector<WorldObject *> *currObjects = &(global::stateManager->currentState->objects);
	while (i < currObjects->size() && (*currObjects)[i]->getID() != obj->getID())
		i++;

	if (i == currObjects->size())
		currObjects->push_back(obj);
	else
		(*currObjects)[i] = obj;
}

ting::Buffer<ting::u8>* NetworkSystem::BuildBuffer(NetworkPacket* packet) {
	int bufSize = packet->dataSize + sizeof(packet->header);
	ting::Buffer<ting::u8> *buf = new ting::Buffer<ting::u8>(new ting::u8[bufSize], bufSize);

	memcpy(buf->Begin(), &packet->header, sizeof(packet->header));
	memcpy(buf->Begin() + sizeof(packet->header), packet->data, packet->dataSize);

	return buf;
}

NetworkPacket* NetworkSystem::ReadBuffer(ting::Buffer<ting::u8>* buf, unsigned int recvSize) {
	// Check if packet can possibly be valid
	if(recvSize <  sizeof(NetworkPacketHeader)) {
		printf("** ReadBuffer: Received Bad Packet! (too small)\n");
		return 0;
	}

	Network::NetworkPacket* packet = new Network::NetworkPacket;
	memcpy(&packet->header, buf->Begin(), sizeof(&packet->header));

	packet->dataSize = recvSize - sizeof(&packet->header);
	packet->data = new unsigned char(packet->dataSize);
	memcpy(packet->data, buf->Begin() + sizeof(&packet->header), packet->dataSize);

	return packet;
}

void NetworkSystem::SendPacket(ting::UDPSocket *socket, ting::IPAddress *serverIP, NetworkPacket* pktPtr) {
	ting::Buffer<ting::u8> *bufPtr;
	bufPtr = BuildBuffer(pktPtr);
	socket->Send(*bufPtr, *serverIP);
	//delete bufPtr;
}

NetworkPacket* NetworkSystem::RecvPacket(ting::UDPSocket *socket, ting::IPAddress *sourceIP) {
	ting::Buffer<ting::u8> buf(new ting::u8[1500], 1500);
	unsigned int recvSize = socket->Recv(buf, *sourceIP);

	NetworkPacket *pktPtr;
	pktPtr = ReadBuffer(&buf, recvSize);

	return pktPtr;
}
