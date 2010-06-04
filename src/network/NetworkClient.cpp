#include "NetworkClient.h"

#include "../graphics/camera.h"

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

void NetworkClient::closeSockets() {
	if(isConnected)
		serverDisconnect();

	if(socket.IsValid()) {
		waitSet->Remove(&socket);
		socket.Close();
		PRINTINFO("Client Socket Closed\n");
	}
}

void NetworkClient::initialize() {}


void NetworkClient::update(long milli_time) {
	updateRxTxData(milli_time);
	//int currServerTime = global::elapsed_ms() - serverClockDelta;

	ting::IPAddress sourceIP;
	NetworkPacket pkt;
	unsigned int pktsRecv = 0;

	static int lagDelayUpdatePeriod = 1000;
	if(isConnected) {
		if(lagDelayUpdatePeriod < 0) {
			lagCalc_StartTime = global::elapsed_ms();
			NetworkPacket tmpPkt(LAG_REQ, (unsigned char *)&lagCalc_StartTime, sizeof(int));
			SendPacket(tmpPkt, &socket, serverIP);
			sendPlayerCamera(camera->position(), camera->viewVec(), &socket, serverIP);
			lagDelayUpdatePeriod = 1000;
		}
		else {
			lagDelayUpdatePeriod -= milli_time;
		}
	}

	// Check for Waiting Network Data
	while(isConnected && waitSet->WaitWithTimeout(0) && pktsRecv < SERVER_RECV_MAX_PACKETS_PER_CYCLE) {
		RecvPacket(&pkt, &socket, &sourceIP);
		int pktRecvTime = global::elapsed_ms();

		if(pkt.header.type == OBJECT_BATCHSEND || pkt.header.type == OBJECT_SEND) {
			// TODO timestamp based interpolation timing
			decodeObjectSend(pkt, 0);
		}
		else if(pkt.header.type == OBJECT_KILL) {
			removeObjectLocal(*(unsigned int*)(pkt.data));
		}
		else if(pkt.header.type == LAG_REPLY) {
			serverDelay = (pktRecvTime - lagCalc_StartTime)/2;
			NetworkPacket tmpPkt(LAG_RESULT, (unsigned char *)&serverDelay, sizeof(int));
			SendPacket(tmpPkt, &socket, serverIP);
		}
		else if(pkt.header.type == LEVEL_CLEAR) {
			global::stateManager->currentState->objects.clear();
		}
		else if(pkt.header.type == STATUS_UPDATE ) {
			myClientID = *(int*)(pkt.data);
			Client::recvClientVectorBinStream(clients, pkt.data+4, pkt.dataSize-4);
/*
			printf("Received Status Update!\n");
			for(unsigned int i=0; i<clients.size(); i++)
				clients[i]->print();
*/
		}
		else {
			printf("Received an unknown packet type!\n");
		}
		pktsRecv++;
	}
}

bool NetworkClient::serverConnect(const char * ip, unsigned int port) {
	serverIP = ting::IPAddress(ip, port);

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
			nextNewObjID = myClientID * 10000;

			serverDelay = (lagCalc_EndTime - lagCalc_StartTime)/2;
			serverClockDelta = global::elapsed_ms() - (*((int*)(pkt.data)+1) + serverDelay);
			Client::recvClientVectorBinStream(clients, pkt.data+8, pkt.dataSize-8);

			printf("Connected! Client %i/%i with a %i ms server delay!\n", myClientID, clients.size(), serverDelay);

			NetworkPacket tmpPkt(LAG_RESULT, (unsigned char *)&serverDelay, sizeof(serverDelay));
			SendPacket(tmpPkt, &socket, serverIP);

			isConnected = true;
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
	}
}

int NetworkClient::getServerDelay() {
	if(!isConnected)
		return -1;

	return serverDelay;
}

void NetworkClient::sendPlayerReady(int readyFlag) {
	global::stateManager->switchToCarnage();	// TODO REMOVE. TEMP FLOW CODE
	if(isConnected) {
		NetworkPacket tmpPkt(PLAYER_READY, (unsigned char *)&readyFlag, sizeof(int));
		SendPacket(tmpPkt, &socket, serverIP);
	}
}

void NetworkClient::sendMsg(char *msgStr) {
	if(isConnected) {
		NetworkPacket tmpPkt(TEXT_MSG, (unsigned char *)msgStr, strlen(msgStr)+1);
		SendPacket(tmpPkt, &socket, serverIP);
		printf("Sent TextMsg.\n");
	}
}

void NetworkClient::addObject(WorldObject *ObjPtr) {
	ObjPtr->setID(nextNewObjID++);
	ObjPtr->setPlayerID(clients[myClientID]->playerID);

	if(isConnected && clients[myClientID]->playerType == Client::PLAYER) {
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
	global::stateManager->currentState->objects.clear();

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
	if(isConnected && clients[myClientID]->playerType == Client::PLAYER) {
		for(unsigned int o = 0; o < newObjs.size(); ++o) {
			// TODO make it so building blocks have a 0-10000 value
			addObject(newObjs[o]);
		}
	}
}
