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

void NetworkClient::update(long milli_time) {
	// Check for Waiting Network Data
	while(isConnected && waitSet->WaitWithTimeout(0)) {
		ting::IPAddress sourceIP;
		NetworkPacket pkt;
		RecvPacket(&pkt, &socket, &sourceIP);
		if(pkt.header.type == OBJECT_SEND) {
			updateLocalObject(new WorldObject(*(WorldObject*)(pkt.data)));
		}
		else {
			printf("Received an unknown packet type!\n");
		}
	}
}


void NetworkClient::sendMsg(char * msgStr) {
	printf("Sending Msg...\n");
	NetworkPacket tmpPkt(TEXT_MSG, (unsigned char *)msgStr, strlen(msgStr)+1);
	SendPacket(tmpPkt, &socket, serverIP);

	socket.Close();
	isConnected = false;
}

void NetworkClient::disconnectServer() {
	unsigned char msg[] = "";
	NetworkPacket tmpPkt(DISCONNECT, msg, sizeof(msg));
	printf("Disconnecting!");
	SendPacket(tmpPkt, &socket, serverIP);
}

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
	if(!waitSet->WaitWithTimeout(1500)) {
		printf("...Connection Timed Out!\n");
		isConnected = false;
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
			printf("...Connected as Player %i!\n", _playerID);
			isConnected = true;
			return true;
		}
		else {
			printf("... Connection Issue!\n");
			isConnected = false;
			return false;
		}
	}
}

void NetworkClient::addObject(WorldObject newObj) {
	newObj.setID(_currObjID++);
	newObj.setPlayerID(_playerID);
	if(isConnected) {
		NetworkPacket pkt(OBJECT_SEND, (unsigned char *)(&newObj), sizeof(newObj));
		SendPacket(pkt, &socket, serverIP);
		newObj.print();
	}

	// Add to local system for interpolation
	updateLocalObject(new WorldObject(newObj));
}
