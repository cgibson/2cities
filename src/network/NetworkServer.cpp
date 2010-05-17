#include "NetworkServer.h"

NetworkServer::NetworkServer() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;
	incomingSock.Open(5060);

	waitSet = new ting::WaitSet(10);
	waitSet->Add(&incomingSock, ting::Waitable::READ);

	_sendObjNext = 0;
	_playerIDNext = 1;

	// Player Specific (since server can be hosted, must include here)
	_playerID = _playerIDNext++;
	_currObjID = _playerID * 10000;
	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine.initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkServer::~NetworkServer() {
	incomingSock.Close();
}

void NetworkServer::initialize() {}

void NetworkServer::update(long milli_time) {
	// Update Physics Engine
	physicsEngine.update(milli_time);

	// UPDATE LOCAL DATA
	std::vector<WorldObject> PhysEngObjs = physicsEngine.getWorldObjects();
	for(int i=0; i < PhysEngObjs.size(); i++) {
		updateLocalObject(new WorldObject(PhysEngObjs[i]));
	}

	// TODO UPDATE DELTAs

	// Incoming Network Section
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;

		// Check for incoming items
		if(waitSet->WaitWithTimeout(0)) {
			printf("-> Packet waiting on a socket!\n");

			if(incomingSock.CanRead()) {
				RecvPacket(&pkt, &incomingSock, &ip);

				if(pkt.header.type == Network::CONN_REQ) {
					printf("Client is requesting to connect!\n");

					// Create new player and add to players
					Player *currPlayer = new Player;
					currPlayer->ID = _playerIDNext++;
					players.push_back(currPlayer);

					// Add Socket/IP to currPlayer
					currPlayer->socket.Open();
					currPlayer->ip = ting::IPAddress(ip.host, ip.port);
					currPlayer->ip.host = 16777343;	// TODO Remove Local Host

					// Add Socket to waitSet
					waitSet->Add(&(currPlayer->socket), ting::Waitable::READ);

					// Send reply so they have new UDP port
					int msg = currPlayer->ID;

					printf("Sending Connection Reply...\n");
					SendPacket(
							&NetworkPacket(CONN_REPLY, (unsigned char*)&msg, sizeof(int)),
							&(currPlayer->socket),
							&(currPlayer->ip));
				}
				else {
					printf("Received an unexpected packet on the server!");
				}
			}

			// check each players[i]->socket->CanRead();
			// TODO Update to NetworkPacket class
			for(int p=0; p<players.size(); p++) {
				if (players[p]->socket.CanRead()) {
					RecvPacket(&pkt, &(players[p]->socket), &ip);
					if(pkt.header.type == OBJECT_SEND) {
						addObjectPhys(*(WorldObject*)(pkt.data));
					}
					else if (pkt.header.type == TEXT_MSG) {
						printf("MSG: %s\n", (char *)pkt.data);
					}
					else {
						printf("Received an unknown packet type!\n");
					}
				}
			}
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
/*
	// Outgoing Network Section
	try {
		vector<WorldObject *> *objList = &global::stateManager->currentState->objects;
		int sendSize = min(50, (int)objList->size());

		if (players.size() > 0) {
			for(int obj=_sendObjNext; obj < _sendObjNext+sendSize; obj=(++obj)% objList->size() ) {
				// Build Packet with an Object
				NetworkPacket pkt(OBJECT_SEND, (unsigned char *)((*objList)[obj]), sizeof(WorldObject));

				//WorldObject tmpObj(*(WorldObject*)(pktPtr->data));
				//printf("id(%i) pos(%s)\n",tmpObj.getID(), tmpObj.getPosition().str());

				// Send to Players
				for(int p=0; p<players.size(); p++) {
					SendPacket(&pkt, &(players[p]->socket), &(players[p]->ip));
				}
			}
			_sendObjNext += sendSize;
		}
	} catch(ting::Socket::Exc &e){
		std::cout << "Network error: " << e.What() << std::endl;
	}
*/
}

void NetworkServer::addObject(WorldObject newObj) {
	newObj.setID(_currObjID++);
	newObj.setPlayerID(_playerID);
	addObjectPhys(newObj);
}

void NetworkServer::addObjectPhys(WorldObject newObj) {
	physicsEngine.addWorldObject(newObj);
	printf("Added item to PhysicsEngine\n");
}

void NetworkServer::loadLevel(const char * file) {
	PRINTINFO("Network Initiated Level in PhysicsEngine\n");
	physicsEngine.loadFromFile(file);
}
