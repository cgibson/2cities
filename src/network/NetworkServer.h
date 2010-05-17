#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../physics/Physics.h"

#include "NetworkSystem.h"
#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"
#include "Player.h"

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

#ifndef MAX_SEND_PACKETS_PER_CYCLE
	#define MAX_SEND_PACKETS_PER_CYCLE 1000
#endif

class NetworkServer : public NetworkSystem {
protected:
	Physics physicsEngine;

	vector<Player *> _players;

	ting::UDPSocket _incomingSock;
	ting::WaitSet *_waitSet;

	vector<WorldObject *> _serverObjs;

	bool _dedicatedServer;
	unsigned int _sendObjNext;
	unsigned int _playerIDNext;

public:
	NetworkServer();
	~NetworkServer();

	void initialize();
	void update(long milli_time);

	void dedicatedServer(bool toggle) {	_dedicatedServer = toggle; }
	bool dedicatedServer(void) { return _dedicatedServer; }

	// Add new object to scene
	void addObject(WorldObject newObj);
	void addObjectPhys(WorldObject newObj);

	void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	void loadLevel(const char * file);
};

#endif
