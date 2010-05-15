#include "NetworkClient.h"

NetworkClient::NetworkClient() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;

	socket = new ting::UDPSocket;
	socket->Open();

	waitSet = new ting::WaitSet(1);
	waitSet->Add(socket, ting::Waitable::READ);

	isConnected = false;
	PRINTINFO("Network Initialized!\n");
}

NetworkClient::~NetworkClient() {}

void NetworkClient::initialize() {}

void NetworkClient::update(long milli_time) {
	// Check for Waiting Network Data
	if(isConnected && waitSet->WaitWithTimeout(0)) {
		ting::IPAddress ip;
		ting::Buffer<ting::u8> buf(new ting::u8[1500], 1500);
		socket->Recv(buf, ip);

		// TODO Check Packet Type

		// If Objects, update local objects received
//		printf("Objects ");
		for(int i=0; i<10; i++) {
			WorldObject *objRecv = new WorldObject(*((WorldObject*)(buf.Begin())+i));
//			printf("%i ", objRecv->getID());
			updateLocalObject(objRecv);
		}
//		printf("Received\n");
	}
}

bool NetworkClient::connectServer(const char * ip, unsigned int port) {
	serverIP = new ting::IPAddress(ip, port);
	Network::NetworkPacket *pktPtr;
	ting::Buffer<ting::u8> *bufPtr;

	unsigned char msg[] = "I'd like to connect!";
	pktPtr = new NetworkPacket(CONN_REQ, msg, sizeof(msg));

	printf("Sending Connection Request...");
	SendPacket(socket, serverIP, pktPtr);
	printf("sent '%s'\n",pktPtr->data);
	//delete pktPtr;

	// Wait for reply response for 1.5 second (1500 ms)
	printf("...Waiting for Server Reply");
	if(!waitSet->WaitWithTimeout(1500)) {
		printf("...Connection Timed Out!\n");
		isConnected = false;
		return false;
	}
	else {
		ting::IPAddress sourceIP;
		pktPtr = RecvPacket(socket, &sourceIP);
		serverIP->port = sourceIP.port;
		printf("...%s\n", pktPtr->data);
		isConnected = true;
		return true;
	}
}

void NetworkClient::addObject(WorldObject newObj) {
	if(isConnected) {
		ting::Buffer<ting::u8> buf(new ting::u8[150], 150);
		memcpy(buf.Begin(), &newObj, sizeof(WorldObject));
		PRINTINFO("Sending Object\n");
		socket->Send(buf, *serverIP);
	}
	global::stateManager->currentState->objects.push_back(new WorldObject(newObj));
}
