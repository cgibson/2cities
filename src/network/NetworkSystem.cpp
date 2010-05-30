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
/*
    // *********************************************
    // Sorted, Linear Iterator Based Update Code
	vector<WorldObject *>::iterator it = objVec->begin();

	while(it != objVec->end() && (*it)->getID() < objPtr->getID()) ++it;

	if(it == objVec->end()) {
//		printf("No Object Found! ID=%i\n",objPtr->getID());
		objVec->push_back(objPtr);
	}
	else if((*it)->getID() == objPtr->getID()) {
//		printf("found item! ID=%i (it=%i)\n",objPtr->getID(), (*it)->getID());
		WorldObject *oldObjPtr = (*it);
		(*it) = objPtr;
		delete oldObjPtr;
	}
	else {
//		printf("new item after %i! ID=%i\n", (*it)->getID(), objPtr->getID());
		objVec->insert(it, objPtr);
	}
	// *********************************************
*/
	// *********************************************
    // Sorted, Binary Search Update Code
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

	// Find Location in main Object vector
	//while (i < objVec->size() && (*objVec)[i]->getID() < objPtr->getID()) { i++; }

	// If at end, must not have found... push to back
	if (i == objVec->size()) {
		objVec->push_back(objPtr);
	}
	// if found, replace pointer with newer one
	else if((*objVec)[i]->getID() == objPtr->getID()) {
		// Check if incoming packet is newer data
//		if (objPtr->getTimeStamp() > (*objVec)[i]->getTimeStamp()) {
			WorldObject *oldObjPtr = (*objVec)[i];
			(*objVec)[i] = objPtr;
			delete oldObjPtr;
//		}
	}
	// Else, not found (but at insert location)
	else {
		objVec->insert(objVec->begin() + i, objPtr);
	}
	// *********************************************

/*
	// *********************************************
    // Non-Sorted, Linear Search Update Code
	unsigned int i=0;
	// Find Location in main Object vector
	while (i < objVec->size() && (*objVec)[i]->getID() != objPtr->getID()) { i++; }

	// if not found, add to end of vector
	if (i == objVec->size()) {
		objVec->push_back(objPtr);
	}
	// if found (and newer), replace pointer with newer one
	else if(objPtr->getTimeStamp() > (*objVec)[i]->getTimeStamp()) {
		WorldObject *oldObjPtr = (*objVec)[i];
		(*objVec)[i] = objPtr;
		delete oldObjPtr;
	}
	// *********************************************
*/
}

/* Method to take a WorldObject* and update/add it to the main vector (based on ID field)
 *
 * Passed Object Pointer must remain alive past function call and shouldn't be deleted
 * until object in later updates OR gameState changes.
 */
void NetworkSystem::updateObjectLocal(WorldObject *objPtr) {
	std::vector<WorldObject *> *currObjects = &(global::stateManager->currentState->objects);
	updateObjectVector(currObjects, objPtr);
}

void NetworkSystem::updatePktData(long elapsed) {
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

/* Method to take a WorldObject* and update/add it to the main vector (based on ID field)
 *
 * Passed Object Pointer must remain alive past function call and shouldn't be deleted
 * until object in later updates OR gameState changes.
 *
 * NOTE: Currently implementation is VERY inefficient **
 * TODO Improve efficiency
 */
void NetworkSystem::removeObjectVector(vector<WorldObject *> *objVec, unsigned int worldObjectID) {
	unsigned int i=0;
	// Find Location in main Object vector
	while (i < objVec->size() && (*objVec)[i]->getID() != worldObjectID) { i++; }

	// if found, erase entry
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
int NetworkSystem::SendPacket(NetworkPacket  pkt, ting::UDPSocket *socket, ting::IPAddress  destIP_P) {
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

int NetworkSystem::readBatchPacket(NetworkPacket *pkt, WorldObject objs[], unsigned int size) {
	if (pkt->header.type != OBJECT_BATCHSEND) {
		printf("readBatchPacket trying to read a non-OBJECT_BATCHSEND packet!\n");
		return -1;
	}
	if (pkt->dataSize % sizeof(WorldObject) != 0) {
		printf("readBatchPacket trying to read a malformed OBJECT_BATCHSEND packet!\n");
		return -1;
	}

	int objCount = pkt->dataSize / sizeof(WorldObject);
	for(int o=0; o<objCount; o++)
		memcpy(&(objs[o]), pkt->data + o*sizeof(WorldObject), sizeof(WorldObject));

	return objCount;
}


void NetworkSystem::decodeObjectSend(NetworkPacket &pkt, long interpValue) {
	if(pkt.header.type == OBJECT_SEND) {
		WorldObject *tmpObjPtr;
		ObjectType woType = *(ObjectType*)(pkt.data);

		switch (woType) {
		default:
			tmpObjPtr = global::factory->makeObject(woType);
			tmpObjPtr->fromBinStream(pkt.data);
		}

		tmpObjPtr->interpolate(interpValue);
		addObjectPhys(tmpObjPtr);
	}
	else if(pkt.header.type == OBJECT_BATCHSEND) {
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
