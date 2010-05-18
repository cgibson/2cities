#include "NetworkServer.h"

NetworkServer::NetworkServer() {
	PRINTINFO("Network Initializing...");
	ting::SocketLib socketsLib;
	_incomingSock.Open(5060);

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
	_playerID = _playerIDNext++;
	_currObjID = _playerID * 10000;
	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine.initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkServer::~NetworkServer() {
	_incomingSock.Close();
}

void NetworkServer::initialize() {}

void NetworkServer::networkIncoming() {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;

		// Check for incoming items
		if(_waitSet->WaitWithTimeout(0)) {
			if(_incomingSock.CanRead()) {
				networkIncomingGeneral();
			}

			// Check each player socket for data (allows up to 10 packets to be accepted)
			for(int p=0; p<_players.size(); p++) {
				int pktsRecv = 0;
				while (_players[p]->socket.CanRead() && pktsRecv++ < 10) {
					networkIncomingPlayers(p);
				}
			}
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::networkIncomingGeneral() {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;

		RecvPacket(&pkt, &_incomingSock, &ip);

		if(pkt.header.type == Network::CONN_REQ) {
			printf("Client is requesting to connect!\n");

			// Create new player and add to players
			Player *currPlayer = new Player;
			currPlayer->ID = _playerIDNext++;
			_players.push_back(currPlayer);

			// Add Socket/IP to currPlayer
			currPlayer->socket.Open();
			currPlayer->ip = ting::IPAddress(ip.host, ip.port);
			currPlayer->ip.host = 16777343;	// TODO Remove Local Host

			// Add Socket to _waitSet
			_waitSet->Add(&(currPlayer->socket), ting::Waitable::READ);

			// Send reply so they have new UDP port
			int msg = currPlayer->ID;

			printf("Sending Connection Reply...\n");
			NetworkPacket tmpPkt(CONN_REPLY, (unsigned char*)&msg, sizeof(int));
			SendPacket(tmpPkt, &(currPlayer->socket), currPlayer->ip);
		}
		else {
			printf("Received an unexpected packet on the server!");
		}
	} catch(ting::Socket::Exc &e) {
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::networkIncomingPlayers(int p) {
	try {
		ting::IPAddress ip;
		NetworkPacket pkt;
		RecvPacket(&pkt, &(_players[p]->socket), &ip);

		switch (pkt.header.type) {
		case OBJECT_SEND :
			addObjectPhys(*(WorldObject*)(pkt.data));
			break;
		case STATUS_CHG :
			printf("STATUS_CHG\n");
			break;
		case DISCONNECT :
			printf("Player #%i is Disconnecting!\n", _players[p]->ID);
			_waitSet->Remove(&_players[p]->socket);
			_players[p]->socket.Close();
			_players.erase(_players.begin()+p);
			exit(1);
			break;
		case LEVEL_LOAD :
			printf("LEVEL_LOAD: %s\n", (char *)pkt.data);
			loadLevel((char *)pkt.data);
			break;
		case TEXT_MSG :
			printf("MSG: %s\n", (char *)pkt.data);
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

void NetworkServer::networkOutgoing() {
	// Outgoing Network Section
	try {
		// Send up to MAX_PACKETS_PER_CYCLE Packets
		int sendSize = min(MAX_SEND_PACKETS_PER_CYCLE, (int)_serverObjs.size());
		if (_players.size() > 0 || _dedicatedServer == false) {
			for(int i=0; i < sendSize; i++) {
				int obj=(_sendObjNext + i)%_serverObjs.size();
				// Add Locally (if hosted)
				if(!_dedicatedServer)
					updateObjectLocal(new WorldObject(*_serverObjs[obj]));

				// Send to _players
				NetworkPacket pkt(OBJECT_SEND, (unsigned char *)(_serverObjs[obj]), sizeof(WorldObject));
				for(int p=0; p<_players.size(); p++) {
					SendPacket(pkt, &(_players[p]->socket), _players[p]->ip);
				}
			}

			if(_serverObjs.size() == 0)
				_sendObjNext = 0;
			else
				_sendObjNext = (_sendObjNext + sendSize)% _serverObjs.size();
		}
	} catch(ting::Socket::Exc &e){
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

void NetworkServer::update(long milli_time) {
	// Update Physics Engine
	physicsEngine.update(milli_time);

	// UPDATE LOCAL DATA
	std::vector<WorldObject> PhysEngObjs = physicsEngine.getWorldObjects();
	for(int i=0; i < PhysEngObjs.size(); i++) {
		updateObjectVector(&_serverObjs,new WorldObject(PhysEngObjs[i]));
	}

	// TODO UPDATE DELTAs

	networkIncoming();
	networkOutgoing();
}

void NetworkServer::addObject(WorldObject newObj) {
	newObj.setID(_currObjID++);
	newObj.setPlayerID(_playerID);
	addObjectPhys(newObj);

	// Add to local system for interpolation
	updateObjectLocal(new WorldObject(newObj));
}

void NetworkServer::addObjectPhys(WorldObject newObj) {
	physicsEngine.addWorldObject(newObj);
}

void NetworkServer::loadLevel(const char * file) {
	physicsEngine.loadFromFile(file);
	PRINTINFO("Network Initiated Level in PhysicsEngine\n");
}
