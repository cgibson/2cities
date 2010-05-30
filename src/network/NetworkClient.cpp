#include "NetworkClient.h"

NetworkClient::NetworkClient() : NetworkSystem() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;

	socket.Open();

	waitSet = new ting::WaitSet(1);
	waitSet->Add(&socket, ting::Waitable::READ);

	isConnected = false;
	_playerID = 0;
	_currObjID = _playerID * 10000;
	PRINTINFO("Network Initialized!\n");
}

NetworkClient::~NetworkClient() {
	closeSockets();
}

void NetworkClient::closeSockets() {
	if(isConnected)
		disconnectServer();

	if(socket.IsValid()) {
		waitSet->Remove(&socket);
		socket.Close();
		PRINTINFO("Client Socket Closed\n");
	}
}

void NetworkClient::initialize() {}

bool NetworkClient::connectServer(const char * ip, unsigned int port) {
	serverIP = ting::IPAddress(ip, port);

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
			_playerID = *(int*)(pkt.data);
			_currObjID = _playerID * 10000;

			serverDelay = (lagCalc_EndTime - lagCalc_StartTime)/2;
			serverTimeDelta = global::elapsed_ms() - (*((int*)(pkt.data)+1) + serverDelay);

			printf("Connected as Player %i with a %i ms server delay!\n", _playerID, serverDelay);

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

void NetworkClient::disconnectServer() {
	if(isConnected) {
		unsigned char msg[] = "";
		NetworkPacket pkt(DISCONNECT, msg, sizeof(msg));
		printf("Disconnecting!\n");
		SendPacket(pkt, &socket, serverIP);

		isConnected = false;
	}
}

int NetworkClient::checkLag(ting::UDPSocket *socket, ting::IPAddress ip) {
	if(socket->IsNotValid()) {
		socket->Open();
	}

	int lagCalc_Delta = -1;
	int lagCalc_EndTime;
	int lagCalc_StartTime = global::elapsed_ms();
	NetworkPacket tmpPkt(LAG_REQ, (unsigned char *)&lagCalc_StartTime, sizeof(int));
	SendPacket(tmpPkt, socket, ip);

	// Wait for reply response for 1 second
	if(waitSet->WaitWithTimeout(1000)) {
		NetworkPacket pkt;
		RecvPacket(&pkt, socket, &ip);
		lagCalc_EndTime = global::elapsed_ms();

		if(pkt.header.type == LAG_REPLY) {
			lagCalc_Delta = (lagCalc_EndTime - lagCalc_StartTime)/2;
			NetworkPacket tmpPkt(LAG_RESULT, (unsigned char *)&lagCalc_Delta, sizeof(int));
			SendPacket(tmpPkt, socket, ip);
		}
	}

	return lagCalc_Delta;
}

void NetworkClient::update(long milli_time) {
	updatePktData(milli_time);
	int currServerTime = global::elapsed_ms() - serverTimeDelta;

	ting::IPAddress sourceIP;
	NetworkPacket pkt;
	unsigned int pktsRecv = 0;

	static int lagDelayUpdatePeriod = 1000;
	if(isConnected) {
		if(lagDelayUpdatePeriod < 0) {
			lagCalc_StartTime = global::elapsed_ms();
			NetworkPacket tmpPkt(LAG_REQ, (unsigned char *)&lagCalc_StartTime, sizeof(int));
			SendPacket(tmpPkt, &socket, serverIP);
			//serverDelay = checkLag(&socket, serverIP);
			//printf("ServerDelay = %i\n",serverDelay);
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
#ifdef DEBUG
			printf("ServerDelay = %i\n",serverDelay);
#endif
			NetworkPacket tmpPkt(LAG_RESULT, (unsigned char *)&serverDelay, sizeof(int));
			SendPacket(tmpPkt, &socket, serverIP);
		}
		else if(pkt.header.type == LEVEL_CLEAR) {
			global::stateManager->currentState->objects.clear();
		}
		else {
			printf("Received an unknown packet type!\n");
		}
		pktsRecv++;
	}
}

void NetworkClient::sendMsg(char * msgStr) {
	if(isConnected) {
		NetworkPacket tmpPkt(TEXT_MSG, (unsigned char *)msgStr, strlen(msgStr)+1);
		SendPacket(tmpPkt, &socket, serverIP);
		printf("Sent TextMsg.\n");
	}
}

void NetworkClient::addObject(WorldObject *ObjPtr) {
	ObjPtr->setID(_currObjID++);
	ObjPtr->setPlayerID(_playerID);

	if(isConnected) {
		unsigned char buf[150];
		int woSize = ObjPtr->makeBinStream(buf);
		NetworkPacket pkt(OBJECT_SEND, buf, woSize);
		SendPacket(pkt, &socket, serverIP);
	}

	// Add to local system for interpolation
	updateObjectLocal(ObjPtr);
}

void NetworkClient::loadLevel(const char * file) {
	if(isConnected) {
		// Clear GameState objects
		global::stateManager->currentState->objects.clear();

		NetworkPacket tmpPkt(LEVEL_LOAD, (unsigned char *)file, strlen(file)+1);
		SendPacket(tmpPkt, &socket, serverIP);
		printf("Sent LoadLevel Request.\n");
	}
}
