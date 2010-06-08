#include "NetworkSystem.h"
#include "../state/CarnageState.h"

#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

/*******************************************
 * CONSTRUCTORS / DESTRUCTORS
 *******************************************/
NetworkSystem::NetworkSystem() {
	_pktCountRecv = _pktCountSent = _pktPeriod = 0;

	myClientID = 0;
	lagCalc_RecvResp = true;

	timeToStateChange = 0;
	timeToStateChangeSet = false;
}

NetworkSystem::~NetworkSystem() {
	closeSockets();
};

/*******************************************
 * Player Details
 *******************************************/
int NetworkSystem::getMyPlayerID() {
	if(clients.size() == 0)
		return 0;
	else
		return clients[myClientID]->playerID;
}

int NetworkSystem::getPlayerReady(int playerID) {
	unsigned int currPlayer = 0;
	while(currPlayer < clients.size()) {
		if(clients[currPlayer]->playerID == playerID)
			return clients[currPlayer]->playerReady;
		currPlayer++;
	}
	return -1;
}

int NetworkSystem::getPlayerScore(int playerID) {
	unsigned int currPlayer = 0;
	while(currPlayer < clients.size()) {
		if(clients[currPlayer]->playerID == playerID)
			return clients[currPlayer]->playerScore;
		currPlayer++;
	}
	return -1;
}

int NetworkSystem::getPlayerDamage(int playerID) {
	unsigned int currPlayer = 0;
	while(currPlayer < clients.size()) {
		if(clients[currPlayer]->playerID == playerID)
			return clients[currPlayer]->playerDamage;
		currPlayer++;
	}
	return -1;
}

void NetworkSystem::setMyPlayerName(char *newName) {
	strncpy(clients[myClientID]->playerName, newName, 15);
	clients[myClientID]->playerName[15] = '\0';
}

void NetworkSystem::setMyPlayerScore(int newScore) {
	clients[myClientID]->playerScore = newScore;
}

void NetworkSystem::setMyPlayerDamage(int newDamage) {
	clients[myClientID]->playerDamage = newDamage;
}

/*******************************************
 * Server/Connect Send/Recv
 *******************************************/
void NetworkSystem::sendServerLagReq(ting::UDPSocket *socketPtr, ting::IPAddress destIP) {
/*
	if(lagCalc_RecvResp == false) {
		serverDisconnect();
		return;
	}
*/
	lagCalc_StartTime = global::elapsed_ms();
	lagCalc_RecvResp = false;
	NetworkPacket tmpPkt(LAG_REQ, (unsigned char *)&lagCalc_StartTime, sizeof(int));
	SendPacket(tmpPkt, socketPtr, destIP);
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

void NetworkSystem::recvMsg(NetworkPacket &pkt) {
#ifdef CLIENT
	gfx::hud.console.info("%s", (char *)pkt.data);
#else
	printf("%s\n", (char *)pkt.data);
#endif
}

/*******************************************
 * Helper/Main Send/Recv Functions
 *******************************************/
// Creates Buffer required for send, sends packet, and cleans up
int NetworkSystem::SendPacket(NetworkPacket &pkt, ting::UDPSocket *socket, ting::IPAddress  destIP_P) {
	if(socket->IsNotValid()) {
		printf("SendPacket> Socket IsNotValid\n");
		return -1;
	}

	int headerSize = sizeof(net::N_PacketType);
	int bufSize = headerSize + pkt.dataSize + sizeof(int);
	ting::u8 strPtr[bufSize];
	ting::Buffer<ting::u8> buf(strPtr, sizeof(strPtr));
	memcpy(buf.Buf()                             , &(pkt.header.type), headerSize);
	memcpy(buf.Buf() + headerSize                , pkt.data          , pkt.dataSize);
	memcpy(buf.Buf() + headerSize + pkt.dataSize , &bufSize          , sizeof(int));

	int sentSize = socket->Send(buf, destIP_P);
	_pktCountSent++;

	return sentSize;
}

// Creates Buffer required for recv, recv packet, and cleans up
// Note: ipPtr's addresses will be updated with packets source's information
int NetworkSystem::RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP_P) {
	if(socket->IsNotValid()) {
		printf("RecvPacket> Socket IsNotValid\n");
		return -1;
	}

	ting::u8 strPtr[2000];
	ting::Buffer<ting::u8> buf(strPtr, sizeof(strPtr));

	unsigned int recvSize = socket->Recv(buf, *srcIP_P);
	_pktCountRecv++;


	int headerSize = sizeof(net::N_PacketType);
	int footerSize = sizeof(int);

	// Sanity Checks
	if((int)(recvSize) < (headerSize + footerSize)) {
		printf("** ReadBuffer: Received Bad Packet! (too small)\n");
		exit(1);
	}
	if(*(unsigned int*)(buf.Buf() + recvSize - footerSize) != recvSize) {
		printf("** ReadBuffer: Received Bad Packet! (size mismatch)\n");
		exit(1);
	}

	pktPtr->dataSize = recvSize - headerSize - footerSize;
	memcpy(&(pktPtr->header.type), buf.Buf()             , headerSize);
	memcpy(  pktPtr->data        , buf.Buf() + headerSize, pktPtr->dataSize);

	return recvSize;
}

/*******************************************
 * vector update/remove helper Functions
 *******************************************/

// Method to take a WorldObject* and update/add it to the main vector (based on ID field.)
// Passed Object Pointer must remain alive past function call and shouldn't be deleted by calling function.
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
			(*objVec)[i]->import(objPtr);
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

/*******************************************
 * Object Sending/Recv
 *******************************************/
void NetworkSystem::buildBatchPacket(NetworkPacket *pkt, WorldObject *objs[], unsigned int size) {
	if (size * 116 > 1400) {
		printf("buildBatchPacket trying to add too much!\n");
		return;
	}

	int currPos = 0;
	for(unsigned int o=0; o<size; o++) {
		currPos += objs[o]->makeBinStream(pkt->data + currPos);
	}
	pkt->header.type = OBJECT_BATCHSEND;
	pkt->dataSize = currPos;

	if(currPos > 1400) {
		printf("Packet Too Large!");
		exit(1);
	}
}

void NetworkSystem::decodeObjectSend(NetworkPacket &pkt, long interpValue) {
	if(pkt.header.type == OBJECT_BATCHSEND || pkt.header.type == OBJECT_SEND || pkt.header.type == LEVEL_BATCHOBJ) {
		WorldObject *tmpObjPtr;
		ObjectType woType;
		unsigned int woPktLoc = 0;

		if(pkt.dataSize > 1400) {
			printf("Packet Corrupt!");
			exit(1);
		}

		while(woPktLoc + 100 < pkt.dataSize) {
			if(woPktLoc + 100 >= pkt.dataSize) {
				printf("Likely Packet Error: Remaining Size < WorldObject Size");
				exit(1);
			}

			woType = *(ObjectType*)(pkt.data + woPktLoc);

			tmpObjPtr = global::factory->makeObject(woType);
			woPktLoc += tmpObjPtr->fromBinStream(pkt.data + woPktLoc);

			tmpObjPtr->interpolate(interpValue);
			addObjectPhys(tmpObjPtr);
			//tmpObjPtr->print();
		}
	}
}

/*******************************************
 * Player Camera Tools Send/Recv
 *******************************************/
int NetworkSystem::makePlayerCamera(Vector  camPos, Vector  camView, unsigned char *bufPtr) {
	int currPos = 0;
	int currISize;

	double tmpD;
	currISize = sizeof(double);

	tmpD = camPos.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = camPos.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = camPos.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;

	tmpD = camView.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = camView.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = camView.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;

	return currPos;
}

void NetworkSystem::sendPlayerCamera(Vector camPos, Vector camView, ting::UDPSocket *socketPtr, ting::IPAddress destIP) {
	NetworkPacket tmpPkt;
	tmpPkt.dataSize = makePlayerCamera(camPos, camView, tmpPkt.data);
	tmpPkt.header.type = CAMLOC_MYLOC;
	SendPacket(tmpPkt, socketPtr, destIP);
}

void NetworkSystem::recvPlayerCamera(Vector &camPos, Vector &camView, unsigned char *bufPtr) {
	camPos = Vector(*((double*)(bufPtr)+0), *((double*)(bufPtr)+1), *((double*)(bufPtr)+2));
	camView = Vector(*((double*)(bufPtr)+3), *((double*)(bufPtr)+4), *((double*)(bufPtr)+5));
}

void NetworkSystem::updOPlayerCamera() {
	if(global::stateManager->currentState->stateType() != CARNAGE_STATE)
		return;

	int myPlayerID = getMyPlayerID();
	unsigned int currClient = 0;
	while(currClient < clients.size() &&
			!(clients[currClient]->playerType == Client::PLAYER &&
					clients[currClient]->playerID != myPlayerID))
		currClient++;

	if(currClient < clients.size()) {
		((CarnageState *)(global::stateManager->currentState))->setOppPos(clients[currClient]->camPos);
		((CarnageState *)(global::stateManager->currentState))->setOppView(clients[currClient]->camView);
	}
}

/*******************************************
 * OBJECT/LEVEL/WORLD FUNCTIONS
 *******************************************/

void NetworkSystem::loadLevel(vector<WorldObject *> newObjs) {
	if(serverConnected() && clients[myClientID]->playerType == Client::PLAYER) {
		int blockStart = (clients[myClientID]->playerID - 1) * 5000 + 1;
		for(unsigned int o = 0; o < newObjs.size(); ++o) {
			// building blocks have a 0-10000 value due to player value 1 or 2
			addObject(newObjs[o], blockStart + o);
		}
	}
}
