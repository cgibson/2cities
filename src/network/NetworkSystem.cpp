#include "NetworkSystem.h"

/* Method to take a WorldObject* and update/add it to the main vector (based on ID field)
 *
 * Passed Object Pointer must remain alive past function call and shouldn't be deleted
 * until object in later updates OR gameState changes.
 *
 * NOTE: Currently implementation is VERY inefficient **
 * TODO Improve efficiency
 */
void NetworkSystem::updateObjectVector(vector<WorldObject *> *objVec, WorldObject *objPtr) {
	int i=0;
	// Find Location in main Object vector
	while (i < objVec->size() && (*objVec)[i]->getID() != objPtr->getID()) { i++; }

	// if not found, add to end of vector
	if (i == objVec->size()) {
		objVec->push_back(objPtr);
	}
	// if found, replace pointer with current one
	else {
		WorldObject *oldObjPtr = (*objVec)[i];
		(*objVec)[i] = objPtr;
		delete oldObjPtr;
	}
}

/* Method to take a WorldObject* and update/add it to the main vector (based on ID field)
 *
 * Passed Object Pointer must remain alive past function call and shouldn't be deleted
 * until object in later updates OR gameState changes.
 */
void NetworkSystem::updateObjectLocal(WorldObject *objPtr) {
	std::vector<WorldObject *> *currObjects = &(global::stateManager->currentState->objects);
	updateObjectVector(currObjects,objPtr);
}

/* Creates Buffer required for send, sends packet, and cleans up
 */
int NetworkSystem::SendPacket(NetworkPacket  pkt, ting::UDPSocket *socket, ting::IPAddress  destIP_P) {
	if(socket->IsNotValid()) {
		printf("Socket Not Open/Valid Yet\n");
		return -1;
	}

//	printf("SendPacket\n");	// TODO DEBUG CODE
//	pkt.display();

	int bufSize = pkt.dataSize + sizeof(NetworkPacketHeader);
	ting::u8 strPtr[bufSize];
	ting::Buffer<ting::u8> buf(strPtr, sizeof(strPtr));
	memcpy(buf.Buf(), &(pkt.header), sizeof(NetworkPacketHeader));
	memcpy(buf.Buf() + sizeof(NetworkPacketHeader), pkt.data, pkt.dataSize);

	int sentSize = socket->Send(buf, destIP_P);
//	printf("Sent Packet of %i bytes\n",sentSize);	// TODO DEBUG CODE

	return sentSize;
}

/* Creates Buffer required for recv, recv packet, and cleans up
 * Note: ipPtr's addresses will be updated with packets source's information
 */
int NetworkSystem::RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP_P) {
	// TODO DEBUG CODE
//	printf("RecvPacket\n");

	ting::u8 strPtr[2000];
	ting::Buffer<ting::u8> buf(strPtr, sizeof(strPtr));

	int recvSize = socket->Recv(buf, *srcIP_P);
//	printf("Recv Packet of %i bytes\n", recvSize);	// TODO DEBUG CODE

	// Check if packet can possibly be valid
	if(recvSize <  sizeof(NetworkPacketHeader)) {
		printf("** ReadBuffer: Received Bad Packet! (too small)\n");
		return -1;
	}

	*pktPtr = NetworkPacket(&buf, recvSize);

	// TODO DEBUG REMOVE
//	printf("RecvPacket->pktPtr: ");
//	pktPtr->display();

	return recvSize;
}
