#include "NetworkClient.h"

#include "../graphics/camera.h"

/*******************************************
 * CONSTRUCTORS / DESTRUCTORS
 *******************************************/
NetworkClient::NetworkClient() : NetworkSystem() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;

	socket.Open();

	waitSet = new ting::WaitSet(1);
	waitSet->Add(&socket, ting::Waitable::READ);

	isConnected = false;
	myClientID = 1;
	nextNewObjID = myClientID * 10000;
	PRINTINFO("Network Initialized!\n");
}

NetworkClient::~NetworkClient() {
	closeSockets();
}

/*******************************************
 * CORE FUNCTIONS
 *******************************************/
void NetworkClient::update(long milli_time) {
	updateRxTxData(milli_time);
	//int currServerTime = global::elapsed_ms() - serverClockDelta;

	// Periodic Lag Checking
	static int delayedUpdateLag = 1000;
	if(isConnected) {
		if(delayedUpdateLag < 0) {
			//printf("Sending Lag Check... RespRecv = %i\n",lagCalc_RecvResp);
			sendServerLagReq(&socket, serverIP);
			delayedUpdateLag = 1000;
		}
		else {
			delayedUpdateLag -= milli_time;
		}
	}

	// Periodic Send Camera Loc
	static int delayedUpdateCamera = 250;
	if(isConnected) {
		if(delayedUpdateCamera < 0) {
			sendPlayerCamera(camera->position(), camera->viewVec(), &socket, serverIP);
			delayedUpdateCamera = 250;
		}
		else {
			delayedUpdateCamera -= milli_time;
		}
	}

	// Check for Waiting Network Data
	networkIncoming(milli_time);
}

/*******************************************
 * PACKET HANDLING FUNCTIONS (INCOMING)
 *******************************************/
void NetworkClient::networkIncoming(long &elapsed) {
	ting::IPAddress sourceIP;
	NetworkPacket pkt;
	unsigned int pktsRecv = 0;
	NetworkPacket tmpPkt;

	if(isConnected && (global::elapsed_ms() - lastTimePktRecv) > 1500) {
		printf("Connection Timed Out! ");
		serverDisconnect();
		return;
	}

	while(isConnected && waitSet->WaitWithTimeout(0) && pktsRecv < SERVER_RECV_MAX_PACKETS_PER_CYCLE) {
		RecvPacket(&pkt, &socket, &sourceIP);
		lastTimePktRecv = global::elapsed_ms();

		switch(pkt.header.type) {
		case OBJECT_BATCHSEND :
		case OBJECT_SEND :
			// TODO timestamp based interpolation timing
			decodeObjectSend(pkt, 0);
			break;
		case OBJECT_KILL :
			removeObjectLocal(*(unsigned int*)(pkt.data));
			break;
		case LAG_REPLY :
			lagCalc_RecvResp = true;
			//printf("RecvResp\n");
			serverDelay = (lastTimePktRecv - lagCalc_StartTime)/2;
			tmpPkt = NetworkPacket(LAG_RESULT, (unsigned char *)&serverDelay, sizeof(int));
			SendPacket(tmpPkt, &socket, serverIP);
			break;
		case LEVEL_CLEAR :
			global::stateManager->currentState->objects.clear();
			break;
		case STATUS_UPDATE :
			recvStatusUpdate(pkt);
			if(clients[myClientID]->playerType == Client::TODIE)
				serverDisconnect();
			break;
		case TEXT_MSG :
			recvMsg(pkt);
			break;
		case DISCONNECT :
			serverDisconnect();
			global::stateManager->changeCurrentState(MENU_STATE);
			break;
		default :
			printf("Received an unknown packet type!\n");
		}
		pktsRecv++;
	}
}

void NetworkClient::recvStatusUpdate(NetworkPacket &pkt) {
	myClientID = *(int*)(pkt.data);

	Client::recvClientVectorBinStream(clients, pkt.data+12, pkt.dataSize-12);
	updOPlayerCamera();

	State serverState = *(State*)(pkt.data+4);
	timeToStateChange = *(int*)(pkt.data+8) + serverClockDelta;
	timeToStateChangeSet = true;

	if(global::stateManager->currentState->stateType() != serverState)
		global::stateManager->changeCurrentState(serverState);
}

/*******************************************
 * SOCKET/CONNECTION/CLEAN-UP FUNCTIONS
 *******************************************/
void NetworkClient::closeSockets() {
	if(isConnected)
		serverDisconnect();

	if(socket.IsValid()) {
		waitSet->Remove(&socket);
		socket.Close();
		PRINTINFO("Client Socket Closed\n");
	}
}

bool NetworkClient::serverConnect(const char * ip, unsigned int port) {
	try {
		serverIP = ting::IPAddress(ip, port);
	} catch(ting::Socket::Exc &e) {
		return false;
	}

//	if(isConnected)
//		serverDisconnect();

	if(socket.IsNotValid()) {
		socket.Open();
	}
	// TODO check if already connected.

	unsigned char msg[] = "";
	NetworkPacket tmpPkt(CONN_REQ, msg, sizeof(msg));

	int lagCalc_StartTime, lagCalc_EndTime;
	lagCalc_StartTime = global::elapsed_ms();
	printf("Sending Connection Request...");
	SendPacket(tmpPkt, &socket, serverIP);

	// Wait for reply response for 1.5 second (1500 ms)
	if(!waitSet->WaitWithTimeout(3000)) {
		printf("Connection Timed Out!\n");
		isConnected = false;
	}
	else {
		ting::IPAddress sourceIP;
		NetworkPacket pkt;
		RecvPacket(&pkt, &socket, &sourceIP);
		lagCalc_EndTime = global::elapsed_ms();

		if(pkt.header.type == CONN_REPLY) {
			serverIP.port = sourceIP.port;
			myClientID = *(int*)(pkt.data);
			Client::recvClientVectorBinStream(clients, pkt.data+8, pkt.dataSize-8);
			updOPlayerCamera();
			nextNewObjID = clients[myClientID]->playerID * 10000;

			serverDelay = (lagCalc_EndTime - lagCalc_StartTime)/2;
			serverClockDelta = global::elapsed_ms() - (*((int*)(pkt.data)+1) + serverDelay);

			printf("Connected! Client %i/%i with a %i ms server delay!\n", myClientID, clients.size(), serverDelay);

			NetworkPacket tmpPkt(LAG_RESULT, (unsigned char *)&serverDelay, sizeof(serverDelay));
			SendPacket(tmpPkt, &socket, serverIP);

			isConnected = true;
			lastTimePktRecv = global::elapsed_ms();
		}
		else {
			printf("Connection Issue!\n");
			isConnected = false;
		}
	}

//	if(!isConnected)
//		socket.Close();

	return isConnected;
}

void NetworkClient::serverDisconnect() {
	if(isConnected) {
		unsigned char msg[] = "";
		NetworkPacket pkt(DISCONNECT, msg, sizeof(msg));
		printf("Disconnecting!\n");
		SendPacket(pkt, &socket, serverIP);

		isConnected = false;
		global::stateManager->changeCurrentState(MENU_STATE);
	}
}

int NetworkClient::getServerDelay() {
	if(!isConnected)
		return -1;

	return serverDelay;
}

/*******************************************
 * OBJECT/LEVEL/WORLD FUNCTIONS
 *******************************************/
void NetworkClient::addObject(WorldObject *ObjPtr, int newID) {
	if(isConnected && clients[myClientID]->playerType == Client::PLAYER) {
		if(newID == -1)
			ObjPtr->setID(nextNewObjID++);
		else
			ObjPtr->setID(newID);
		ObjPtr->setPlayerID(clients[myClientID]->playerID);

		unsigned char buf[150];
		int woSize = ObjPtr->makeBinStream(buf);
		NetworkPacket pkt(OBJECT_SEND, buf, woSize);
		SendPacket(pkt, &socket, serverIP);
	}

	// Add to local system for interpolation
	updateObjectLocal(ObjPtr);
}

void NetworkClient::emptyWorld() {
	// Clear GameState objects
	InGameState * currState = global::stateManager->currentState;
	for(int i=currState->objects.size() - 1; i >= 0 ; --i) {
		delete currState->objects[i];  // call WorldObject destructor
		currState->objects.pop_back(); // remove and call pointer destructor
	}

	if(isConnected && clients[myClientID]->playerType == Client::PLAYER) {
		int tmpInt = 0;
		NetworkPacket tmpPkt(LEVEL_CLEAR, (unsigned char *)&tmpInt, sizeof(int));
		SendPacket(tmpPkt, &socket, serverIP);
		printf("Sent ClearWorld Request.\n");
	}
}

void NetworkClient::loadLevel(const char * file) {
	// Clear GameState objects
	global::stateManager->currentState->objects.clear();

	if(isConnected && clients[myClientID]->playerType == Client::PLAYER) {
		NetworkPacket tmpPkt(LEVEL_LOAD, (unsigned char *)file, strlen(file)+1);
		SendPacket(tmpPkt, &socket, serverIP);
		printf("Sent LoadLevel Request.\n");
	}
}

void NetworkClient::loadLevel(vector<WorldObject *> newObjs) {
	if(serverConnected() && clients[myClientID]->playerType == Client::PLAYER) {
		int blockStart = (clients[myClientID]->playerID - 1) * 1000 + 1;

		unsigned int objSetStart = 0;
		while(objSetStart < newObjs.size()) {
			// Build Batch Packet
			WorldObject *objPtrGroup[net::OBJECT_BATCHSEND_SIZE];

			unsigned int SetPos = 0;
			while((SetPos < net::OBJECT_BATCHSEND_SIZE) && (objSetStart + SetPos) < newObjs.size()) {
				objPtrGroup[SetPos] = newObjs[objSetStart + SetPos];
				objPtrGroup[SetPos]->setPlayerID(clients[myClientID]->playerID);
				objPtrGroup[SetPos]->setID(blockStart++);
				SetPos++;
			}

			NetworkPacket pkt;
			buildBatchPacket(&pkt, objPtrGroup, SetPos);
			pkt.header.type = LEVEL_BATCHOBJ;
			SendPacket(pkt, &socket, serverIP);

			// If no response, resend until response.
			bool pktConfirm = false;
			ting::IPAddress sourceIP;
			NetworkPacket tmpPkt;
			int sendAttempt = 0;

			while(!pktConfirm) {
				if(!waitSet->WaitWithTimeout(20)) {
					if(sendAttempt > 10) {
						printf("Hit max attempts\n");
						return;
					}
					else {
						SendPacket(pkt, &socket, serverIP);
						sendAttempt++;
#ifdef DEBUG
						printf("Re-sending Level_Batch. Attempt %i!\n", sendAttempt);
#endif
					}
				}
				else {
					while(waitSet->WaitWithTimeout(0)) {
						RecvPacket(&tmpPkt, &socket, &sourceIP);
						lastTimePktRecv = global::elapsed_ms();
						if(tmpPkt.header.type == LEVEL_BATCHOBJ) {
							if(tmpPkt.dataSize == pkt.dataSize && !memcmp(tmpPkt.data, pkt.data, pkt.dataSize)) {
#ifdef DEBUG
								printf("Level Packet Successful!\n");
#endif
								pktConfirm = true;
							}
#ifdef DEBUG
							else
								printf("Packet Didn't Match!\n");
#endif
						}
#ifdef DEBUG
						else if(tmpPkt.header.type == STATUS_UPDATE)
							printf("Status Packet... Ignoring!\n");
						else
							printf("Other Packet... Ignoring!\n");
#endif
					}
				}
			}
			objSetStart += SetPos;
		}

		int readyFlag = 0;
		NetworkPacket tmpPkt(PLAYER_READY, (unsigned char *)&readyFlag, sizeof(int));
		SendPacket(tmpPkt, &socket, serverIP);
		printf("Done Sending\n");
	}
}

/*******************************************
 * PLAYER INTERACTION FUNCTIONS
 *******************************************/
void NetworkClient::setPlayerReady(int readyFlag) {
	if(isConnected) {
		NetworkPacket tmpPkt(PLAYER_READY, (unsigned char *)&readyFlag, sizeof(int));
		SendPacket(tmpPkt, &socket, serverIP);
	}
}

void NetworkClient::setMyPlayerName(char *newName) {
	NetworkPacket tmpPkt(PLAYER_NAME, (unsigned char *)(newName) , strlen(newName)+1);
	SendPacket(tmpPkt, &socket, serverIP);
}

void NetworkClient::setMyPlayerScore(int newScore) {
	if(isConnected) {
		NetworkPacket tmpPkt(SCORE_MINE, (unsigned char *)&newScore, sizeof(int));
		SendPacket(tmpPkt, &socket, serverIP);
	}
}

void NetworkClient::setMyPlayerDamage(int newDamage) {
	if(isConnected) {
		NetworkPacket tmpPkt(DAMAGE_MINE, (unsigned char *)&newDamage, sizeof(int));
		SendPacket(tmpPkt, &socket, serverIP);
	}
}

void NetworkClient::sendMsg(char *msgStr) {
	if(isConnected) {
		NetworkPacket tmpPkt(TEXT_MSG, (unsigned char *)(msgStr) , 0);
		tmpPkt.dataSize = sprintf((char *)(tmpPkt.data),"%s: %s",clients[myClientID]->playerName, msgStr);
		tmpPkt.data[tmpPkt.dataSize] = '\0';
		tmpPkt.dataSize++;

		SendPacket(tmpPkt, &socket, serverIP);
	}
}
