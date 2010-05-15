#include "NetworkServer.h"

NetworkServer::NetworkServer() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;
	incomingSock = new ting::UDPSocket;
	incomingSock->Open(5060);

	waitSet = new ting::WaitSet(10);
	waitSet->Add(incomingSock, ting::Waitable::READ);

	lastGroup = 0;
	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine = new Physics();
	physicsEngine->initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkServer::~NetworkServer() {}

void NetworkServer::initialize() {}

void NetworkServer::update(long milli_time) {
	// Update Physics Engine
	physicsEngine->update(milli_time);

	// UPDATE LOCAL DATA
	std::vector<WorldObject> PhysEngObjs = physicsEngine->getWorldObjects();
	for(int i=0; i < PhysEngObjs.size(); i++) {
		updateLocalObject(new WorldObject(PhysEngObjs[i]));
	}

	// TODO UPDATE DELTAs

	// Incoming Network Section
	try {
		// Check for incoming items
		if(waitSet->WaitWithTimeout(0)) {
			printf("-> Packet waiting on some Socket!\n");

			if(incomingSock->CanRead()) {
				ting::IPAddress ip;
				Network::NetworkPacket *pktPtr;
				pktPtr = RecvPacket(incomingSock, &ip);

				printf("check packet details\n");
				if(pktPtr != 0 && pktPtr->header.type == Network::CONN_REQ) {
					printf("Client is requesting to connect!\n");

					// Create new player and add to players
					Player *currPlayer = new Player;
					players.push_back(currPlayer);

					static int newPlayerID = 1;
					currPlayer->ID = newPlayerID++;

					// Add Socket/IP to currPlayer
					currPlayer->socket = new ting::UDPSocket;
					currPlayer->socket->Open();
					currPlayer->ip = new ting::IPAddress(ip.host, ip.port);
					currPlayer->ip->host = 16777343;	// TODO Remove Local Host

					// Add Socket to waitSet
					waitSet->Add(currPlayer->socket, ting::Waitable::READ);

					// Send reply so they have new UDP port
					unsigned char msg[] = "";
					pktPtr = new NetworkPacket(CONN_REPLY, msg, sizeof(msg));

					printf("Sending Connection Reply...\n");
					SendPacket(currPlayer->socket, currPlayer->ip, pktPtr);
					printf(" <- Replied to %i:%i\n",currPlayer->ip->host, currPlayer->ip->port);
				}
				else {
					printf("Received an unexpected packet on the server!");
				}
			}

			// check each players[i]->socket->CanRead();
			// TODO Update to NetworkPacket class
			for(int p=0; p<players.size(); p++) {
				if (players[p]->socket->CanRead()) {
					ting::Buffer<ting::u8> buf(new ting::u8[1500], 1500);
					ting::IPAddress ip;

					players[p]->socket->Recv(buf, ip);
					WorldObject newObj(*(WorldObject*)(buf.Begin()));
					printf("Received Obj #%i\n", newObj.getID());
					addObject(newObj);
				}
			}
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}

	// Outgoing Network Section
	try {
		// send data to clients
		const int groupSize = 10;
		const int packetSize = groupSize * sizeof(WorldObject);
		ting::Buffer<ting::u8> buf(new ting::u8[packetSize], packetSize);

		InGameState *currState = global::stateManager->currentState;
		int objectSize = currState->objects.size();
		int sendSize = min(50, objectSize);

		if (players.size() > 0) {
			for(int o=0; o < sendSize; o+=groupSize) {
				// TODO build packet
				for(int i=0; i<groupSize; i++) {
					int currObj = (o+i+lastGroup)%objectSize;
//					printf("%i ",currState->objects[currObj]->getID());
					memcpy((WorldObject*)(buf.Begin())+i, currState->objects[currObj], sizeof(WorldObject));
				}
				// Send to all clients
//				printf("... -> Sending to Player ");
				for(int p=0; p<players.size(); p++) {
					if(!players[p]->socket->IsValid()) {
						printf("Socket !IsValid!\n");
					}
//					printf("#%i ", players[p]->ID);
					players[p]->socket->Send(buf, *(players[p]->ip));
				}
//				printf("\n");
			}
//			printf("...Group Send Complete!\n");
			lastGroup += sendSize;
		}
	} catch(ting::Socket::Exc &e){
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::addObject(WorldObject newObj) {
	physicsEngine->addWorldObject(newObj);
}

void NetworkServer::loadLevel(const char * file) {
	PRINTINFO("Network Initiated Level in PhysicsEngine\n");
	physicsEngine->loadFromFile(file);
}
