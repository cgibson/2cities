#include "NetworkServer.h"

#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

/*******************************************
 * CONSTRUCTORS / DESTRUCTORS
 *******************************************/
NetworkServer::NetworkServer() {
	PRINTINFO("Network Initializing...");

	try {
		ting::SocketLib socketsLib;
		serverSocket.Open(net::SERVER_PORT_DEFAULT);

		_waitSet = new ting::WaitSet(10);
		_waitSet->Add(&serverSocket, ting::Waitable::READ);
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
		exit(0); // TODO do something more
	}

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
	timeToStateChangePause = false;
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
			networkIncoming(elapsed);	// SERVER
		}

		currClockTime = global::elapsed_ms();
		if(currClockTime != lastClockTime) {
			elapsed = currClockTime - lastClockTime;
			lastClockTime = currClockTime;
#else
	networkIncoming(elapsed);	// CLIENT
#endif

	updateRxTxData(elapsed);
	checkStateChange();
	checkClientTimeout();

	if(global::stateManager->currentState->stateType() == CARNAGE_STATE)
		updatePlayerDetails();

	// Update Physics Engine
	static int physicsDelay = 0;
	if(physicsDelay <= 0) {
		int mainRunTime = (net::SERVER_PHYSICS_UPDATE_RATE - physicsDelay);
		physicsEngine.update(mainRunTime);
#ifdef SERVER
		int physRunTime = (global::elapsed_ms() - currClockTime);
		printServerStatus(mainRunTime, physRunTime);
#endif
		physicsDelay = net::SERVER_PHYSICS_UPDATE_RATE;

		getPhysicsUpdate();
	}
	else {
		physicsDelay -= elapsed;
	}

	WorldObjectState::sortVector(&_serverObjs);
	networkOutgoing(elapsed);

#ifdef SERVER
		} // END IF TIME PASSED
	} // END SERVER WHILE
#endif
}

void NetworkServer::printServerStatus(int mainLoopTime, int phyLoopTime) {
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
	printf("LCycle(%4i) Phys(%4i) tx(%5i) rx(%5i) objs(%4i) ttsc(%4i) | ",
			mainLoopTime,
			phyLoopTime,
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
}

void NetworkServer::getPhysicsUpdate() {
	// UPDATE LOCAL DATA (and remove items fallen off world)
	std::vector<WorldObject *> PhysEngObjs = physicsEngine.getWorldObjects();
	for(unsigned int i=0; i < PhysEngObjs.size(); i++) {
/*
		if(PhysEngObjs[i]->getPosition().y() < -50.0f) {
			unsigned int removeID = PhysEngObjs[i]->getID();
			// Local Removal
			physicsEngine.removeWorldObject(removeID);
			WorldObjectState::removeVector(&_serverObjs,removeID);
#ifdef CLIENT
			removeObjectLocal(removeID);
#endif

			// Send to Clients
			NetworkPacket pkt(OBJECT_KILL, (unsigned char *)&removeID, sizeof(unsigned int));
			for(unsigned int p=0; p<clients.size(); p++) {
				if(!clients[p]->isLocal)
					SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
			}
		}
		else {
*/
			// Needed to add new items
			WorldObjectState::updateVector(&_serverObjs,PhysEngObjs[i]);
//		}
	}
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
				unsigned int pktsRecv = 0;
				while (!clients[p]->isLocal && clients[p]->socket.CanRead() &&
						pktsRecv++ < net::SERVER_RECV_MAX_PACKETS_PER_CYCLE)
				{
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
		case LEVEL_BATCHOBJ :
			// Send Packet back as confirmation
			SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
		case OBJECT_SEND :
		case OBJECT_BATCHSEND :
			decodeObjectSend(pkt, 0); // TODO clients[p]->playerDelay);
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
		case SCORE_MINE :
			clients[p]->playerScore = *(int*)(pkt.data);
			break;
		case DAMAGE_MINE :
			clients[p]->playerDamage = *(int*)(pkt.data);
			break;
		case PLAYER_READY :
			clients[p]->playerReady = *(int*)(pkt.data);
			printf("PLAYER_READY: P#%i C#%i Ready=%i\n", clients[p]->playerID, p, clients[p]->playerReady);
			break;
		case PLAYER_NAME :
			strncpy(clients[p]->playerName, (char *)(pkt.data), 16);
			printf("PLAYER_NAME: P#%i C#%i %s\n", clients[p]->playerID, p, clients[p]->playerName);
			break;
		case CAMLOC_MYLOC :
			recvPlayerCamera(clients[p]->camPos, clients[p]->camView, pkt.data);
#ifdef CLIENT
			updOPlayerCamera();
#endif
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
		const int vecSize = _serverObjs.size();
		//const int sendSize = min(vecSize, (int)(SERVER_SEND_MAX_PACKETS_PER_MS * elapsed * OBJECT_BATCHSEND_SIZE));
		const int MaxObjsCycle = (int)(OBJECT_BATCHSEND_SIZE * SERVER_SEND_MAX_PACKETS_PER_CYCLE);
		const int MaxObjsTime  = (int)(OBJECT_BATCHSEND_SIZE * SERVER_SEND_MAX_PACKETS_PER_MS * elapsed);
		const int sendSize = min(vecSize, min(MaxObjsCycle, MaxObjsTime));
		if (sendSize > 0 && ((clients.size() > 1 || (_dedicatedServer == true && clients.size() > 0)))) {
			int objSetStart = 0;
			int currTime = global::elapsed_ms();
			while(objSetStart < sendSize) {
				unsigned int objSetSize = min(vecSize - objSetStart, (int)OBJECT_BATCHSEND_SIZE);

				// Build Batch Packet
				WorldObject *objPtrGroup[net::OBJECT_BATCHSEND_SIZE];

				unsigned int SetPos = 0;
				unsigned int SetItem = 0;
				while((SetItem < objSetSize)) { //  && (objSetStart + SetPos < vecSize)
					if(_serverObjs[objSetStart + SetItem].objPtr != NULL) {
						objPtrGroup[SetPos] = _serverObjs[objSetStart + SetItem].objPtr;
						_serverObjs[objSetStart + SetItem].sent(currTime);
						++SetPos;
					}
					++SetItem;
				}

				if(SetPos > 0) {
					NetworkPacket pkt;
					buildBatchPacket(&pkt, objPtrGroup, SetPos);

					// Send to each _players
					for(unsigned int p=0; p<clients.size(); p++) {
						if(!clients[p]->isLocal) {
							// Avoid Flooding Clients Sending Level Vectors
							if(clients[p]->playerReady >= 0)
								SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
						}
						else
							decodeObjectSend(pkt, 0);
					}
				}

				objSetStart += SetItem;
			}
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
	NetworkPacket tmpPkt(DISCONNECT, (unsigned char *)&clientID, sizeof(int));
	SendPacket(tmpPkt, &(clients[clientID]->socket), clients[clientID]->ip);

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
		if(!clients[i]->isLocal && clients[i]->lastPktRecv < minLastPktRecv) {
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
			if(!clients[p]->isLocal) { // && clients[p]->playerReady >= 0) {
				memcpy(pkt.data, (unsigned char *)&p, sizeof(unsigned int));
				SendPacket(pkt, &(clients[p]->socket), clients[p]->ip);
			}
		}
	}
}

void NetworkServer::checkStateChange() {
	int readyCount = 0;
	int playerCount = 0;

	for(unsigned int i=0; i<clients.size(); ++i) {
		if(clients[i]->playerType == Client::PLAYER) {
			++playerCount;
			if(clients[i]->playerReady == 1)
				++readyCount;
		}
	}

	State currState = global::stateManager->currentState->stateType();

	// If timer is not set, then set it.
	if(!timeToStateChangeSet || (playerCount == 0)) {
		switch (currState) {
		case MENU_STATE :
			timeToStateChange = global::elapsed_ms() + 9999 * 1000;
			timeToStateChangeSet = false;
			timeToStateChangePause = true;
			break;
		case BUILD_STATE :
			//timeToStateChange = global::elapsed_ms() + net::TIME_IN_BUILD_STATE * 1000;
			//timeToStateChangeSet = true;

			timeToStateChange = global::elapsed_ms() + 9999 * 1000;
			timeToStateChangeSet = false;
			timeToStateChangePause = true;
			break;
		case CARNAGE_STATE :
			timeToStateChange = global::elapsed_ms() + net::TIME_IN_CARNAGE_STATE * 1000;
			timeToStateChangeSet = true;
			break;
		case RESULTS_STATE :
			timeToStateChange = global::elapsed_ms() + net::TIME_IN_RESULTS_STATE * 1000;
			timeToStateChangeSet = true;
			break;
		default :
			timeToStateChange = global::elapsed_ms() + 9999 * 1000;
			timeToStateChangeSet = false;
			timeToStateChangePause = true;
		}
	}

	bool stateChange = false;
	stateChange |= (playerCount == readyCount && playerCount == 2);
	stateChange |= (timeToStateChangeSet && (timeToStateChange - global::elapsed_ms()) <= 0 && !timeToStateChangePause);
	stateChange |= (currState == CARNAGE_STATE && checkWinCondition());
	stateChange |= (currState != BUILD_STATE && playerCount == 0);
	stateChange |= (currState == MENU_STATE);  // Should only happen in CLIENT build

	if(stateChange) {
		// Clear Ready Flags
		for(unsigned int i=0; i<clients.size(); ++i)
			clients[i]->playerReady = 0;

		// Change State to next in line.
		switch (currState) {
		case MENU_STATE :
			global::stateManager->changeCurrentState(BUILD_STATE);
			emptyWorld();
			break;
		case BUILD_STATE :
			for(unsigned int i=0; i<clients.size(); ++i)
				if(!clients[i]->isLocal)
					clients[i]->playerReady = -1;

			global::stateManager->changeCurrentState(CARNAGE_STATE);
			break;
		case CARNAGE_STATE :
			global::stateManager->changeCurrentState(RESULTS_STATE);
			break;
		case RESULTS_STATE :
			if(playerCount != readyCount || playerCount != 2) {
				// send disconnect command to clients (kick 'em out)
				for(unsigned int i=0; i < clients.size(); ++i) {
					NetworkPacket tmpPkt(DISCONNECT, (unsigned char *)&i, sizeof(int));
					SendPacket(tmpPkt, &(clients[i]->socket), clients[i]->ip);
					clients[i]->playerType = Client::TODIE;
					clients[i]->playerID   = 0;
				}
			}

			global::stateManager->changeCurrentState(BUILD_STATE);
			emptyWorld();
			break;
		default :
			printf("What State am I In?");
		}
		timeToStateChangeSet = false;
		timeToStateChangePause = false;
	}
}

int NetworkServer::checkWinCondition() {
	int playerCount = 0;
	for(unsigned int i=0; i<clients.size(); ++i) {
		if(clients[i]->playerType == Client::PLAYER) {
			++playerCount;
			if(clients[i]->playerDamage >= DAMAGE_PERCENT_WIN_CONDITION) {
				return clients[i]->playerID;
			}
		}
	}

	return (playerCount != 2);
}

void NetworkServer::updatePlayerDetails() {
	// NOTE This function will only check blocks with ID < 10000 since those
	// are building blocks instead of ammo.

	// TODO DAMAGE:
	// check each object to see if its moved from original location

	// TODO SCORE:
	// check each object to see if its moved from its original location &
	// add it to a score multiplier queue of when this occured

	int blocksP1 = 0;
	int blocksP2 = 0;

	int damagedP1 = 0;
	int damagedP2 = 0;

	for(unsigned int i=0; i < _serverObjs.size(); ++i) {
		if(_serverObjs[i].objID < 10000) {
			// Count All Objects
			switch (_serverObjs[i].playerID) {
			case 1 :
				++blocksP1;
				break;
			case 2 :
				++blocksP2;
				break;
			}

			// Count Damaged Objs
			if(_serverObjs[i].damaged) {
				switch (_serverObjs[i].playerID) {
				case 1 :
					++damagedP1;
					break;
				case 2 :
					++damagedP2;
					break;
				}
			}
		}
	}

	if(blocksP1 == 0) { blocksP1 = 1; }
	if(blocksP2 == 0) { blocksP2 = 1; }
	for(unsigned int i=0; i < clients.size(); ++i) {
		if(clients[i]->playerID == 2) {
			clients[i]->playerDamage = (int)(damagedP1*100/blocksP1);
			clients[i]->playerScore  = damagedP1 * 1024;
		}
		else if(clients[i]->playerID == 1) {
			clients[i]->playerDamage = (int)(damagedP2*100/blocksP2);
			clients[i]->playerScore  = damagedP2 * 1024;
		}
	}
}

/*******************************************
 * OBJECT/LEVEL/WORLD FUNCTIONS
 *******************************************/
void NetworkServer::addObject(WorldObject *objPtr, int newID) {
	if(newID == -1)
		objPtr->setID(nextNewObjID++);
	else
		objPtr->setID(newID);

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

	WorldObject * tmpObjPtr = global::factory->makeObject(objPtr->getType());
	tmpObjPtr->import(objPtr);
	
	// Add ObjectState to Tracker
	WorldObjectState::updateVector(&_serverObjs, tmpObjPtr, 10);

	physicsEngine.addWorldObject(tmpObjPtr);
}

void NetworkServer::emptyWorld() {
	// Clear GameState objects
	// Note: clear okay since Physics will do actual WO deletes
	global::stateManager->currentState->objects.clear();

	// Clear Server objects
	_serverObjs.clear(); // Can call just clear since they are not pointers

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

