#include "NetworkSystem.h"

/* Method to take a WorldObject* and update/add it to the main vector (based on ID field)
 *
 * Passed Object Pointer must remain alive past function call and shouldn't be deleted
 * by calling function.
 */
void NetworkSystem::updateObjectVector(vector<WorldObject *> *objVec, WorldObject *objPtr) {
    // *** Binary Search Code ***
	int locBeg = 0;
	int locEnd = objVec->size() - 1;
	int locMid = (locBeg + locEnd) / 2;
	while(locBeg < locEnd) {
		// Check if found object
		if ((*objVec)[locMid]->getID() == objPtr->getID()) {
			locBeg = locMid;	// Found, break out!
			break;
		}
		// Must be in last half
		else if ((*objVec)[locMid]->getID() < objPtr->getID()) {
			locBeg = locMid + 1;
		}
		// Must be in first half
		else {
			locEnd = locMid - 1;
		}
		locMid = (locBeg + locEnd) / 2;
	}
	if(objVec->size() != 0 && (*objVec)[locBeg]->getID() < objPtr->getID()) locBeg++;

	unsigned int i = locBeg;

	// *** Linear Search Code ***
	//while (i < objVec->size() && (*objVec)[i]->getID() < objPtr->getID()) { i++; }

	// *** Add, Insert, Update Code ***
	// ObjID Not Found (and at end)... Push_back
	if (i == objVec->size()) {
		objVec->push_back(objPtr);
	}
	// ObjID Found, Replace Data
	else if((*objVec)[i]->getID() == objPtr->getID()) {
		// Is new item newer? If not, ignore
		if (objPtr->getTimeStamp() > (*objVec)[i]->getTimeStamp()) {
			(*objVec)[i]->import(*objPtr);
			delete objPtr;
		}
	}
	// ObjID Not Found, thus Insert Item
	else {
		objVec->insert(objVec->begin() + i, objPtr);
	}
}

void NetworkSystem::updateObjectLocal(WorldObject *objPtr) {
	std::vector<WorldObject *> *currObjects = &(global::stateManager->currentState->objects);
	updateObjectVector(currObjects, objPtr);
}

void NetworkSystem::updateRxTxData(long elapsed) {
	_pktPeriod += elapsed;
	if(_pktPeriod > 250) {
		global::pbs_recv = _pktCountRecv * 4;
		global::pbs_sent = _pktCountSent * 4;

		//printf("pbs> Recv: %i Sent: %i\n",global::pbs_recv,global::pbs_sent);

		_pktCountRecv = 0;
		_pktCountSent = 0;
		_pktPeriod = 0;
	}
}

/* Method to take a WorldObjectID and remove it from a vector
 *
 * NOTE: Currently search implementation could be replaced with binary search inefficient **
 */
void NetworkSystem::removeObjectVector(vector<WorldObject *> *objVec, unsigned int worldObjectID) {
    // *** Binary Search Code ***
	// If more complex code desired, see updateObjectVector Code

	unsigned int i=0;

	// *** Linear Search Code ***
	while (i < objVec->size() && (*objVec)[i]->getID() != worldObjectID) { i++; }

	// ObjID found, erase entry
	if (i < objVec->size()) {
		objVec->erase(objVec->begin()+i);
	}
}

void NetworkSystem::removeObjectLocal(unsigned int worldObjectID) {
	std::vector<WorldObject *> *currObjects = &(global::stateManager->currentState->objects);
	removeObjectVector(currObjects, worldObjectID);
}

/* Creates Buffer required for send, sends packet, and cleans up
 */
int NetworkSystem::SendPacket(NetworkPacket &pkt, ting::UDPSocket *socket, ting::IPAddress  destIP_P) {
	if(socket->IsNotValid()) {
		printf("SendPacket> Socket IsNotValid\n");
		return -1;
	}

	int bufSize = pkt.dataSize + sizeof(NetworkPacketHeader);
	ting::u8 strPtr[bufSize];
	ting::Buffer<ting::u8> buf(strPtr, sizeof(strPtr));
	memcpy(buf.Buf(), &(pkt.header), sizeof(NetworkPacketHeader));
	memcpy(buf.Buf() + sizeof(NetworkPacketHeader), pkt.data, pkt.dataSize);

	int sentSize = socket->Send(buf, destIP_P);
	_pktCountSent++;

	return sentSize;
}

/* Creates Buffer required for recv, recv packet, and cleans up
 * Note: ipPtr's addresses will be updated with packets source's information
 */
int NetworkSystem::RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP_P) {
	if(socket->IsNotValid()) {
		printf("RecvPacket> Socket IsNotValid\n");
		return -1;
	}

	ting::u8 strPtr[2000];
	ting::Buffer<ting::u8> buf(strPtr, sizeof(strPtr));

	unsigned int recvSize = socket->Recv(buf, *srcIP_P);
	_pktCountRecv++;

	// Check if packet can possibly be valid
	if(recvSize <  sizeof(NetworkPacketHeader)) {
		printf("** ReadBuffer: Received Bad Packet! (too small)\n");
		return -1;
	}

	*pktPtr = NetworkPacket(&buf, recvSize);

	return recvSize;
}

void NetworkSystem::buildBatchPacket(NetworkPacket *pkt, WorldObject *objs[], unsigned int size) {
	if (size * sizeof(WorldObject) > 1500) {
		printf("buildBatchPacket trying to add too much!\n");
		return;
	}

	int currPos = 0;
	for(unsigned int o=0; o<size; o++) {
		currPos += objs[o]->makeBinStream(pkt->data + currPos);
	}
	pkt->header.type = OBJECT_BATCHSEND;
	pkt->dataSize = currPos;
}

void NetworkSystem::decodeObjectSend(NetworkPacket &pkt, long interpValue) {
	if(pkt.header.type == OBJECT_BATCHSEND || pkt.header.type == OBJECT_SEND) {
		WorldObject *tmpObjPtr;
		ObjectType woType;
		int woPktLoc = 0;

		while(woPktLoc < pkt.dataSize) {
			woType = *(ObjectType*)(pkt.data + woPktLoc);

			switch (woType) {
			default:
				tmpObjPtr = global::factory->makeObject(woType);
				woPktLoc += tmpObjPtr->fromBinStream(pkt.data + woPktLoc);
			}

			//tmpObjPtr->print();
			tmpObjPtr->interpolate(interpValue);
			addObjectPhys(tmpObjPtr);
		}
	}
}
