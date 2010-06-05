#include "NetworkServer.h"

#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

/*******************************************
 * CONSTRUCTORS / DESTRUCTORS
 *******************************************/
NetworkServer::NetworkServer() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;
	serverSocket.Open(net::SERVER_PORT_DEFAULT);

	_waitSet = new ting::WaitSet(10);
	_waitSet->Add(&serverSocket, ting::Waitable::READ);

#ifdef CLIENT
	_dedicatedServer = false;
#else
	_dedicatedServer = true;
#endif
	_sendObjNext = 0;

// Player Specific (since server can be hosted, must include here)
#ifdef CLIENT
	myClientID = 0;
	char playerName[] = "Host Player";	// TODO check pkt.data for name
	clients.push_back(new Client(1,playerName, true));
	clients[myClientID]->playerType = Client::PLAYER;
	nextNewObjID = clients[myClientID]->playerID * 10000;
#else
	myClientID = -1;
	nextNewObjID = 0;
#endif
	timeToStateChange = global::elapsed_ms() - 1; // Will be a long time until it circles around;
	timeToStateChangeSet = false;
	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine.initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkServer::~NetworkServer() {
	closeSockets();
}

/*******************************************
 * CORE FUNCTIONS
 *******************************************/
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
	checkStateChange();
	checkClientTimeout();

	// Update Physics Engine
	static int physicsDelay = 0;
	if(physicsDelay <= 0) {
		physicsEngine.update(net::SERVER_PHYSICS_UPDATE_RATE - physicsDelay);

#ifdef SERVER
		// Print Server Stats for Dedicated Server Only
		switch (global::stateManager->currentState->stateType()) {
		case MENU_STATE :
			printf("\015   MENU ");
			break;
		case BUILD_STATE :
			printf("\015  BUILD ");
			break;
		case CARNAGE_STATE :
			printf("\015   CARN ");
			break;
		case RESULTS_STATE :
			printf("\015 RESULT ");
			break;
		default :
			printf("\015    N/A ");
		}
		printf("LCycle(%4i) Phys(%4li) tx(%5i) rx(%5i) objs(%4i) ttsc(%4i) | ",
				(net::SERVER_PHYSICS_UPDATE_RATE - physicsDelay),
				(global::elapsed_ms() - currClockTime),
				global::pbs_sent,
				global::pbs_recv,
				_serverObjs.size(),
				(timeToStateChange - global::elapsed_ms()) / 1000);
		for(unsigned int p=0; p<clients.size(); p++) {
			printf("P%i(%4i ms) ", clients[p]->playerID, clients[p]->playerDelay);
		}
		printf("   ");
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
				for(unsigned int p=0; p<clients.size(); p++) {
					if(!clients[p]->isLocal)
						SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
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

/*******************************************
 * PACKET HANDLING FUNCTIONS (INCOMING)
 *******************************************/
void NetworkServer::networkIncoming(long &elapsed) {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;

		// Check for incoming items
		if(_waitSet->WaitWithTimeout(0)) {
			if(serverSocket.CanRead()) {
				networkIncomingGeneral(elapsed);
			}

			// Check each player socket for data (allows up to 10 packets to be accepted)
			for(unsigned int p=0; p<clients.size(); p++) {
				int pktsRecv = 0;
				while (!clients[p]->isLocal && clients[p]->socket.CanRead() && pktsRecv++ < 10) {
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

		RecvPacket(&pkt, &serverSocket, &ip);

		if(pkt.header.type == net::CONN_REQ) {
			printf("Client is requesting to connect!\n");

			// Create new player and add to players
			char playerName[] = "Rem Player";	// TODO check pkt.data for name
			Client *currPlayer = new Client(0, playerName);
			clients.push_back(currPlayer);

			int playerValue = 1;
			for(unsigned int i=0; i<clients.size(); i++) {
				if(clients[i]->playerType == Client::PLAYER) {
					if(clients[i]->playerID == playerValue)
						playerValue++;
				}
			}
			if(playerValue <= 2) {
				currPlayer->playerType = Client::PLAYER;
				currPlayer->playerID = playerValue;
			}
			currPlayer->lastPktRecv = global::elapsed_ms();

			// Add Socket/IP to currPlayer
			currPlayer->socket.Open();
			currPlayer->ip = ting::IPAddress(ip.host>>24, (ip.host<<8)>>24, (ip.host<<16)>>24, (ip.host<<24)>>24, ip.port);

			// Add Socket to _waitSet
			_waitSet->Add(&(currPlayer->socket), ting::Waitable::READ);

			// Send reply so they have new UDP port, playerID, and serverClock
			unsigned char msg[sizeof(int) * 2];
			int clientID = clients.size()-1;
			memcpy(msg, (unsigned char*)&clientID, sizeof(int));
			int gameClock = global::elapsed_ms();
			memcpy(msg + sizeof(int), (unsigned char*)&gameClock, sizeof(int));

			printf("Sending Connection Reply (gameClock = %i)...\n", gameClock);
			NetworkPacket tmpPkt(CONN_REPLY, msg, sizeof(int)*2);
			tmpPkt.dataSize = Client::makeClientVectorBinStream(clients, tmpPkt.data+8) + 8;

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
		RecvPacket(&pkt, &(clients[p]->socket), &ip);

		int gameClock;
		clients[p]->lastPktRecv = global::elapsed_ms();

		switch (pkt.header.type) {
		case OBJECT_SEND :
			decodeObjectSend(pkt, clients[p]->playerDelay);
			break;
		case LAG_REQ :
			gameClock = global::elapsed_ms();
			pkt = NetworkPacket (LAG_REPLY, (unsigned char *)&gameClock, sizeof(int));
			SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
			break;
		case LAG_RESULT :
			clients[p]->playerDelay = *(int*)(pkt.data);
			break;
		case DISCONNECT :
			playerDisconnect(p);
			break;
		case LEVEL_CLEAR :
			emptyWorld();
			break;
		case LEVEL_LOAD :
			printf("LEVEL_LOAD: %s\n", (char *)pkt.data);
			loadLevel((char *)pkt.data);
			break;
		case PLAYER_READY :
			clients[p]->playerReady = *(int*)(pkt.data);
			printf("PLAYER_READY: %i\n", clients[p]->playerReady);
			break;
		case PLAYER_NAME :
			strncpy(clients[p]->playerName, (char *)(pkt.data), 16);
			printf("PLAYER_NAME: %s\n", clients[p]->playerName);
			break;
		case CAMLOC_MYLOC :
			recvPlayerCamera(clients[p]->camPos, clients[p]->camView, pkt.data);
#ifdef CLIENT
			updOPlayerCamera();
#endif
			//printf("Player #%i CamPos%s CamView%s\n",_players[p]->ID,_players[p]->camPos.str(),_players[p]->camView.str());
			break;
		case TEXT_MSG :
			recvMsg(pkt);
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

/*******************************************
 * PACKET HANDLING FUNCTIONS (OUTGOING)
 *******************************************/
void NetworkServer::networkOutgoing(long &elapsed) {
	// Outgoing Network Section
	try {
		sendStatusUpdates();

		// Send up to MAX_PACKETS_PER_CYCLE Packets
		const int objsSize = _serverObjs.size();
		const int sendSize = min(objsSize, (int)(SERVER_SEND_MAX_PACKETS_PER_MS * elapsed * OBJECT_BATCHSEND_SIZE));
		if (sendSize > 0 && ((clients.size() > 1 || (_dedicatedServer == true && clients.size() > 0)))) {
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
				for(unsigned int p=0; p<clients.size(); p++) {
					if(!clients[p]->isLocal)
						SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
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

/*******************************************
 * SOCKET/CONNECTION/CLEAN-UP FUNCTIONS
 *******************************************/
void NetworkServer::closeSockets() {
	serverSocket.Close();
	PRINTINFO("Main Server Port Closed\n");

	for(unsigned int p=0; p<clients.size(); p++) {
		clients[p]->socket.Close();
	}
	PRINTINFO("Player Ports Closed\n");
}

void NetworkServer::playerDisconnect(int clientID) {
	printf("%s (C#%i P#%i) is Disconnecting!\n",
			clients[clientID]->playerName,
			clientID,
			clients[clientID]->playerID);
	_waitSet->Remove(&clients[clientID]->socket);
	clients[clientID]->socket.Close();
	clients.erase(clients.begin() + clientID);
}

void NetworkServer::checkClientTimeout() {
	int minLastPktRecv = global::elapsed_ms() - CLIENT_TIMEOUT;
	unsigned int i = 0;
	while(i < clients.size()) {
		if(clients[i]->lastPktRecv < minLastPktRecv) {
			printf("Client Timed Out! ");
			playerDisconnect(i);
		}
		else
			++i;
	}
}

/*******************************************
 * STATUS/STATE CHECKING FUNCTIONS
 *******************************************/
void NetworkServer::sendStatusUpdates() {
	// Send STATUS update to each _players
	if((clients.size() > 1 || (_dedicatedServer == true && clients.size() > 0))) {
		NetworkPacket pkt;
		pkt.dataSize = Client::makeClientVectorBinStream(clients, pkt.data+12) + 12;
		State serverState = global::stateManager->currentState->stateType();
		memcpy(pkt.data+4, (unsigned char *)&serverState, sizeof(State));
		memcpy(pkt.data+8, (unsigned char *)&timeToStateChange, sizeof(int));
		pkt.header.type = STATUS_UPDATE;
		for(unsigned int p=0; p<clients.size(); p++) {
			if(!clients[p]->isLocal) {
				memcpy(pkt.data, (unsigned char *)&p, sizeof(unsigned int));
				SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
			}
		}
	}
}

void NetworkServer::checkStateChange() {
	State currState = global::stateManager->currentState->stateType();
	int readyCount = 0;
	int playerCount = 0;

	for(unsigned int i=0; i<clients.size(); ++i) {
		if(clients[i]->playerType == Client::PLAYER) {
			++playerCount;
			if(clients[i]->playerReady == 1)
				++readyCount;
		}
	}

	timeToStateChangeSet = (playerCount > 0);

	if(!timeToStateChangeSet) {
		switch (currState) {
		case MENU_STATE :
			timeToStateChangeSet = false;
			break;
		case BUILD_STATE :
			timeToStateChange = global::elapsed_ms() + net::TIME_IN_BUILD_STATE * 1000;
			break;
		case CARNAGE_STATE :
			timeToStateChange = global::elapsed_ms() + net::TIME_IN_CARNAGE_STATE * 1000;
			break;
		case RESULTS_STATE :
			timeToStateChange = global::elapsed_ms() + net::TIME_IN_RESULTS_STATE * 1000;
			break;
		default :
			timeToStateChangeSet = false;
		}
	}

	bool stateChange = false;
	stateChange |= (playerCount == readyCount && playerCount > 0);
	stateChange |= (timeToStateChangeSet && (timeToStateChange - global::elapsed_ms()) <= 0);
	//stateChange |= (currState == CARNAGE_STATE && checkWinCondition());

	if(stateChange) {
		// Clear Ready Flags
		for(unsigned int i=0; i<clients.size(); ++i)
			clients[i]->playerReady = 0;

		// Change State to next in line.
		switch (currState) {
		case MENU_STATE :
			global::stateManager->changeCurrentState(BUILD_STATE);
			break;
		case BUILD_STATE :
			global::stateManager->changeCurrentState(CARNAGE_STATE);
			break;
		case CARNAGE_STATE :
			global::stateManager->changeCurrentState(RESULTS_STATE);
			break;
		case RESULTS_STATE :
			global::stateManager->changeCurrentState(MENU_STATE);
			break;
		default :
			printf("What State am I In?");
		}
		timeToStateChangeSet = false;
	}
}

int NetworkServer::checkWinCondition() {
	for(unsigned int i=0; i<clients.size(); ++i) {
		if(clients[i]->playerType == Client::PLAYER) {
			if(clients[i]->playerDamage >= 95) {
				return clients[i]->playerID;
			}
		}
	}

	return false;
}

/*******************************************
 * OBJECT/LEVEL/WORLD FUNCTIONS
 *******************************************/
void NetworkServer::addObject(WorldObject *objPtr) {
	objPtr->setID(nextNewObjID++);
	if(myClientID == -1)
		objPtr->setPlayerID(0);
	else
		objPtr->setPlayerID(clients[myClientID]->playerID);
	
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

void NetworkServer::emptyWorld() {
	// Clear GameState objects
	global::stateManager->currentState->objects.clear();

	// Clear Server objects
	_serverObjs.clear();

	// Clear Physics
	physicsEngine.emptyWorld();

	// Send to Clients
	char msg[] = "";
	NetworkPacket pkt(LEVEL_CLEAR, (unsigned char *)&msg, sizeof(msg));
	for(unsigned int p=0; p<clients.size(); p++) {
		if(!clients[p]->isLocal)
			SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
	}
}

void NetworkServer::loadLevel(const char * file) {
	emptyWorld();

	// Load level (which will clear PhysicsEngine objects)
	physicsEngine.loadFromFile(file);
	PRINTINFO("Network Initiated Level in PhysicsEngine\n");
}

/*******************************************
 * PLAYER INTERACTION FUNCTIONS
 *******************************************/
void NetworkServer::sendMsg(char *msgStr) {
	NetworkPacket tmpPkt(TEXT_MSG, (unsigned char *)msgStr, strlen(msgStr)+1);
	recvMsg(tmpPkt);
}

void NetworkServer::recvMsg(NetworkPacket &pkt) {
#ifdef CLIENT
	gfx::hud.console.info("%s", (char *)pkt.data);
#else
	printf("%s\n", (char *)pkt.data);
#endif

	for(unsigned int p=0; p<clients.size(); p++) {
		if(!clients[p]->isLocal)
			SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
	}
}

void NetworkServer::setPlayerReady(int readyFlag) {
	clients[myClientID]->playerReady = 1;
}

