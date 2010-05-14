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
		printf("Objects ");
		for(int i=0; i<10; i++) {
			WorldObject *objRecv = new WorldObject(*((WorldObject*)(buf.Begin())+i));
			printf("%i ", objRecv->getID());
			updateLocalObject(objRecv);
		}
		printf("Received\n");
	}
}

bool NetworkClient::connectServer(const char * ip, unsigned int port) {
	serverIP = new ting::IPAddress(ip, port);
	ting::Buffer<ting::u8> buf(new ting::u8[1500], 1500);
	strcpy((char*)buf.Begin(),"I'd like to connect!");

	printf("Sending Connection Request...");
	socket->Send(buf, *serverIP);

	// Wait for reply response for 1.5 second (1500 ms)
	printf("...Waiting for Server Reply");
	if(!waitSet->WaitWithTimeout(1500)) {
		printf("...Connection Timed Out!\n");
		isConnected = false;
		return false;
	}
	else {
		socket->Recv(buf, *serverIP);
		serverIP->host = 16777343;		// TODO remove whe
		printf("...%s\n", buf.Begin());
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
}
