#include "NetworkServer.h"

bool NetworkPrioritySort(NetworkObjectState objSt1, NetworkObjectState objSt2) {
	if(objSt1.priority != objSt2.priority)
		return (objSt1.priority > objSt2.priority);

	double obj1Vel = objSt1.objPtr->getVelocity().mag();
	double obj2Vel = objSt2.objPtr->getVelocity().mag();
	if(abs(obj1Vel - obj2Vel) > 0.5f) //(5*net::timeElapsed/1000.0f))
		return (obj1Vel > obj2Vel);

	double obj1PosDelta = (objSt1.objPtr->getPosition() - objSt1.lastPos).mag();
	double obj2PosDelta = (objSt2.objPtr->getPosition() - objSt2.lastPos).mag();
	if(abs(obj1PosDelta - obj2PosDelta) > 0.5f) //(5*net::timeElapsed/1000.0f))
		return (obj1PosDelta > obj2PosDelta);

	return (objSt1.lastUpdate < objSt2.lastUpdate);
}

NetworkServer::NetworkServer() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;
	_incomingSock.Open(net::SERVER_PORT_DEFAULT);

	_waitSet = new ting::WaitSet(10);
	_waitSet->Add(&_incomingSock, ting::Waitable::READ);

#ifdef CLIENT
	_dedicatedServer = false;
#else
	_dedicatedServer = true;
#endif
	_sendObjNext = 0;
	_playerIDNext = 1;

// Player Specific (since server can be hosted, must include here)
#ifdef CLIENT
	_playerID = _playerIDNext++;
#else
	_playerID = 0;
#endif
	_currObjID = _playerID * 10000;
	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine.initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkServer::~NetworkServer() {
	closeSockets();
}

void NetworkServer::closeSockets() {
	_incomingSock.Close();
	PRINTINFO("Main Server Port Closed\n");

	for(unsigned int p=0; p<_players.size(); p++) {
		_players[p]->socket.Close();
	}
	PRINTINFO("Player Ports Closed\n");
}

void NetworkServer::initialize() {}

int  NetworkServer::checkLag(ting::UDPSocket *socket, ting::IPAddress ip) {
/*
	try {
		NetworkPacket pkt;

		RecvPacket(&pkt, &_incomingSock, &ip);

		if(pkt.header.type == net::LAG_REQ) {
			// Add Socket/IP to currPlayer
			socket->Open();
			ip = ting::IPAddress(ip.host>>24, (ip.host<<8)>>24, (ip.host<<16)>>24, (ip.host<<24)>>24, ip.port);

			// Add Socket to _waitSet
			_waitSet->Add(&(currPlayer->socket), ting::Waitable::READ);

			// Send reply so they have new UDP port, playerID, and serverClock
			unsigned char msg[sizeof(int)];
			int gameClock = global::elapsed_ms();
			memcpy(msg, (unsigned char*)&gameClock, sizeof(int));

			NetworkPacket tmpPkt(LAG_REPLY, msg, sizeof(int));
			SendPacket(tmpPkt, &(currPlayer->socket), currPlayer->ip);
		}
		else {
			printf("Received an unexpected packet on the server!");
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
*/
	return -1;
}

void NetworkServer::networkIncoming(long &elapsed) {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;

		// Check for incoming items
		if(_waitSet->WaitWithTimeout(0)) {
			if(_incomingSock.CanRead()) {
				networkIncomingGeneral(elapsed);
			}

			// Check each player socket for data (allows up to 10 packets to be accepted)
			for(unsigned int p=0; p<_players.size(); p++) {
				int pktsRecv = 0;
				while (_players[p]->socket.CanRead() && pktsRecv++ < 10) {
					networkIncomingPlayers(p, elapsed);
				}
			}
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::networkIncomingGeneral(long &elapsed) {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;

		RecvPacket(&pkt, &_incomingSock, &ip);

		if(pkt.header.type == net::CONN_REQ) {
			printf("Client is requesting to connect!\n");

			// Create new player and add to players
			Player *currPlayer = new Player;
			currPlayer->ID = _playerIDNext++;
			currPlayer->camPos = Vector(0,100,0);
			currPlayer->camView = Vector(0,100,0);
			currPlayer->lagDelay = 0;
			currPlayer->playerReady = 0;
			currPlayer->playerScore = 0;
			int playerCount = 0;
			for(unsigned int i=0; i<_players.size(); i++)
				if(_players[i]->playerType == PLAYER)
					playerCount++;
			if(playerCount < 2)
				currPlayer->playerType = PLAYER;
			else
				currPlayer->playerType = SPECTATOR;
			_players.push_back(currPlayer);

			// Add Socket/IP to currPlayer
			currPlayer->socket.Open();
			currPlayer->ip = ting::IPAddress(ip.host>>24, (ip.host<<8)>>24, (ip.host<<16)>>24, (ip.host<<24)>>24, ip.port);

			// Add Socket to _waitSet
			_waitSet->Add(&(currPlayer->socket), ting::Waitable::READ);

			// Send reply so they have new UDP port, playerID, and serverClock
			unsigned char msg[sizeof(int) * 2];
			int playerID = currPlayer->ID;
			memcpy(msg, (unsigned char*)&playerID, sizeof(int));
			int gameClock = global::elapsed_ms();
			memcpy(msg + sizeof(int), (unsigned char*)&gameClock, sizeof(int));

			printf("Sending Connection Reply (gameClock = %i)...\n", gameClock);
			NetworkPacket tmpPkt(CONN_REPLY, msg, sizeof(int)*2);
			SendPacket(tmpPkt, &(currPlayer->socket), currPlayer->ip);
		}
		else {
			printf("Received an unexpected packet on the server!");
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::networkIncomingPlayers(int p, long &elapsed) {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;
		RecvPacket(&pkt, &(_players[p]->socket), &ip);

		int gameClock;

		switch (pkt.header.type) {
		case OBJECT_SEND :
			decodeObjectSend(pkt, _players[p]->lagDelay);
			break;
		case LAG_REQ :
			gameClock = global::elapsed_ms();
			pkt = NetworkPacket (LAG_REPLY, (unsigned char *)&gameClock, sizeof(int));
			SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
			break;
		case LAG_RESULT :
			_players[p]->lagDelay = *(int*)(pkt.data);
			break;
		case DISCONNECT :
			printf("Player #%i is Disconnecting!\n", _players[p]->ID);
			_waitSet->Remove(&_players[p]->socket);
			_players[p]->socket.Close();
			_players.erase(_players.begin()+p);
			break;
		case LEVEL_LOAD :
			printf("LEVEL_LOAD: %s\n", (char *)pkt.data);
			loadLevel((char *)pkt.data);
			break;
		case PLAYER_READY :
			_players[p]->playerReady = *(int*)(pkt.data);
			printf("PLAYER_READY: %i\n", _players[p]->playerReady);
			break;
		case CAMLOC_MYLOC :
			recvPlayerCamera(_players[p]->camPos, _players[p]->camView, pkt.data);
			//printf("Player #%i CamPos%s CamView%s\n",_players[p]->ID,_players[p]->camPos.str(),_players[p]->camView.str());
			break;
		case TEXT_MSG :
			printf("MSG: %s\n", (char *)pkt.data);
			exit(0);
			break;
		case UNKNOWN :
		default :
			printf("No Rule for Packet Received!\n");
			break;
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::networkOutgoing(long &elapsed) {
	// Outgoing Network Section
	try {
		/*
		// Send STATUS update to each _players
		if((_players.size() > 0 || _dedicatedServer == false)) {
			NetworkPacket pkt;
			for(unsigned int p=0; p<_players.size(); p++) {
				SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
			}
		}
		*/

		// Send up to MAX_PACKETS_PER_CYCLE Packets
		const int objsSize = _serverObjs.size();
		const int sendSize = min(objsSize, (int)(SERVER_SEND_MAX_PACKETS_PER_MS * elapsed * OBJECT_BATCHSEND_SIZE));
		if (sendSize > 0 && (_players.size() > 0 || _dedicatedServer == false)) {
			unsigned int currObj = _sendObjNext;
			while(currObj < _sendObjNext + sendSize) {
				// Update Locally
				for(int o=0; o<10; o++)
					updateObjectLocal(new WorldObject(*_serverObjs[(currObj+o)%objsSize]));

				// Build Batch Packet
				WorldObject *objPtrGroup[OBJECT_BATCHSEND_SIZE];
				for(unsigned int o=0; o<OBJECT_BATCHSEND_SIZE; o++)
					objPtrGroup[o] = _serverObjs[(currObj+o)%objsSize];
				NetworkPacket pkt;
				buildBatchPacket(&pkt, objPtrGroup, OBJECT_BATCHSEND_SIZE);

				// Send to each _players
				for(unsigned int p=0; p<_players.size(); p++) {
					SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
				}
				currObj += OBJECT_BATCHSEND_SIZE;
			}

			if(objsSize == 0)
				_sendObjNext = 0;
			else
				_sendObjNext = (_sendObjNext + sendSize)%objsSize;
		}
	} catch(ting::Socket::Exc &e){
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::update(long elapsed) {
#ifdef SERVER
	static long lastClockTime = global::elapsed_ms();
	long currClockTime;
	while(1) {
		if(_waitSet->WaitWithTimeout(1)) {
#endif
	networkIncoming(elapsed);
#ifdef SERVER
		}
		else {
			currClockTime = global::elapsed_ms();
			if(currClockTime != lastClockTime) {
				elapsed = currClockTime - lastClockTime;
				lastClockTime = currClockTime;
#endif

	updateRxTxData(elapsed);

	// Update Physics Engine
	static int physicsDelay = 0;
	if(physicsDelay <= 0) {
		physicsEngine.update(net::SERVER_PHYSICS_UPDATE_RATE - physicsDelay);

#ifdef SERVER
		// Print Server Stats for Dedicated Server Only
		printf("\015 LCycle(%4i) Phys(%4li) tx(%5i) rx(%5i) objs(%4i) | ",
				(net::SERVER_PHYSICS_UPDATE_RATE - physicsDelay),
				(global::elapsed_ms() - currClockTime),
				global::pbs_sent,
				global::pbs_recv,
				_serverObjs.size());
		for(unsigned int p=0; p<_players.size(); p++) {
			printf("P%i(%4i ms) ", _players[p]->ID, _players[p]->lagDelay);
		}
		printf("  ");
		fflush(stdout);
#endif

		physicsDelay = net::SERVER_PHYSICS_UPDATE_RATE;

		// UPDATE LOCAL DATA (and remove items fallen off world)
		std::vector<WorldObject *> PhysEngObjs = physicsEngine.getWorldObjects();
		for(unsigned int i=0; i < PhysEngObjs.size(); i++) {
			if(PhysEngObjs[i]->getPosition().y() < -50.0f) {
				unsigned int removeID = PhysEngObjs[i]->getID();
				// Local Removal
				physicsEngine.removeWorldObject(removeID);
				removeObjectVector(&_serverObjs,removeID);
				removeObjectLocal(removeID);

				// Send to Clients
				NetworkPacket pkt(OBJECT_KILL, (unsigned char *)&removeID, sizeof(unsigned int));
				for(unsigned int p=0; p<_players.size(); p++) {
					SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
				}
			}
			else {
				updateObjectVector(&_serverObjs,new WorldObject(*PhysEngObjs[i]));
			}
		}
	}
	else {
		physicsDelay -= elapsed;
	}

	networkOutgoing(elapsed);
#ifdef SERVER
			}
		}
	}
#endif
}

void NetworkServer::addObject(WorldObject *objPtr) {
	objPtr->setID(_currObjID++);
	objPtr->setPlayerID(_playerID);
	
	addObjectPhys(objPtr);

	// Add to local vector for rendering
	updateObjectLocal(objPtr);
}

void NetworkServer::addObjectPhys(WorldObject *objPtr) {
	objPtr->setTimeStamp(global::elapsed_ms());
	
	// TODO Add ObjectState to Tracker
	// NetworkObjectState newObjState(objPtr, 4);
	
	physicsEngine.addWorldObject(objPtr);
}

void NetworkServer::loadLevel(const char * file) {
	// Clear GameState objects
	global::stateManager->currentState->objects.clear();

	// Clear Server objects
	_serverObjs.clear();

	// Send to Clients
	char msg[] = "";
	NetworkPacket pkt(LEVEL_CLEAR, (unsigned char *)&msg, sizeof(msg));
	for(unsigned int p=0; p<_players.size(); p++) {
		SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
	}

	// Load level (which will clear PhysicsEngine objects)
	physicsEngine.loadFromFile(file);
	PRINTINFO("Network Initiated Level in PhysicsEngine\n");
}
/*
void NetworkServer::loadLevel(vector<WorldObject *> newObjs) {
	// Clear GameState objects
	global::stateManager->currentState->objects.clear();

	// Clear Server objects
	_serverObjs.clear();

	// Send to Clients
	char msg[] = "";
	NetworkPacket pkt(LEVEL_CLEAR, (unsigned char *)&msg, sizeof(msg));
	for(unsigned int p=0; p<_players.size(); p++) {
		SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
	}

	for(int o = 0; o < newObjs.size(); ++o) {
		// TODO make it so building blocks have a 0-10000 value
		addObject(newObjs[o]);
	}
	PRINTINFO("Network Initiated Level in PhysicsEngine\n");
}
*/
