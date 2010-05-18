#include "NetworkClient.h"

NetworkClient::NetworkClient() {
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
	disconnectServer();
}

void NetworkClient::initialize() {}

bool NetworkClient::connectServer(const char * ip, unsigned int port) {
	serverIP = ting::IPAddress(ip, port);

	if(socket.IsNotValid()) {
		socket.Open();
	}

	unsigned char msg[] = "";
	NetworkPacket tmpPkt(CONN_REQ, msg, sizeof(msg));
	printf("Sending Connection Request...");
	SendPacket(tmpPkt, &socket, serverIP);

	// Wait for reply response for 1.5 second (1500 ms)
	if(!waitSet->WaitWithTimeout(3000)) {
		printf("Connection Timed Out!\n");
		isConnected = false;
		socket.Close();
		return false;
	}
	else {
		ting::IPAddress sourceIP;
		NetworkPacket pkt;
		RecvPacket(&pkt, &socket, &sourceIP);

		if(pkt.header.type == CONN_REPLY) {
			serverIP.port = sourceIP.port;
			_playerID = *(int*)(pkt.data);
			_currObjID = _playerID * 10000;

			printf("Connected as Player %i!\n", _playerID);
			isConnected = true;
			return true;
		}
		else {
			printf("Connection Issue!\n");
			isConnected = false;
			socket.Close();
			return false;
		}
	}
}

void NetworkClient::disconnectServer() {
	unsigned char msg[] = "";
	NetworkPacket pkt(DISCONNECT, msg, sizeof(msg));
	printf("Disconnecting!");
	SendPacket(pkt, &socket, serverIP);

	socket.Close();
	isConnected = false;
}

void NetworkClient::update(long milli_time) {
	ting::IPAddress sourceIP;
	NetworkPacket pkt;
	int pktsRecv = 0;

	// Check for Waiting Network Data
	while(isConnected && waitSet->WaitWithTimeout(0) && pktsRecv < MAX_RECV_PACKETS_PER_CYCLE) {
		RecvPacket(&pkt, &socket, &sourceIP);
		if(pkt.header.type == OBJECT_SEND) {
			updateObjectLocal(new WorldObject(*(WorldObject*)(pkt.data)));
		}
		else {
			printf("Received an unknown packet type!\n");
		}
		pktsRecv++;
	}
//	printf("Received %i packets\n", pktsRecv);
}

void NetworkClient::sendMsg(char * msgStr) {
	NetworkPacket tmpPkt(TEXT_MSG, (unsigned char *)msgStr, strlen(msgStr)+1);
	SendPacket(tmpPkt, &socket, serverIP);
	printf("Sent TextMsg.\n");
}

void NetworkClient::addObject(WorldObject newObj) {
	printf("addObject:\n");
	newObj.setID(_currObjID++);
	newObj.setPlayerID(_playerID);

	if(isConnected) {
		NetworkPacket pkt(OBJECT_SEND, (unsigned char *)(&newObj), sizeof(WorldObject));
		SendPacket(pkt, &socket, serverIP);
	}

	// Add to local system for interpolation
	updateObjectLocal(new WorldObject(newObj));
	newObj.print();
}

void NetworkClient::loadLevel(const char * file) {
	NetworkPacket tmpPkt(LEVEL_LOAD, (unsigned char *)file, strlen(file)+1);
	SendPacket(tmpPkt, &socket, serverIP);
	printf("Sent LoadLevel Request.\n");
}
