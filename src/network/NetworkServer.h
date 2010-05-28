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

class NetworkObjectState {
public:
	WorldObject *objPtr;

	// Initial Details
	Vector initPos;

	// Delta Details
	int    lastUpdate;
	Vector lastPos;

	// Priority (to be reduced each send down to zero)
	int    priority;

	NetworkObjectState() {
		objPtr     = NULL;
		lastUpdate = 0;
		lastPos    = Vector(0,0,0);
		initPos    = Vector(0,0,0);
	}

	NetworkObjectState(WorldObject * newObj, int initPriority = 2) {
		objPtr     = newObj;
		initPos    = newObj->getPosition();
		lastPos    = initPos;
		lastUpdate = newObj->getTimeStamp();
		priority   = initPriority;
	}
};

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

	void networkIncoming(long &elapsed);
	void networkIncomingGeneral(long &elapsed);
	void networkIncomingPlayers(int p, long &elapsed);
	void networkOutgoing(long &elapsed);

public:
	NetworkServer();
	~NetworkServer();
	void closeSockets();

	int  checkLag(ting::UDPSocket *socket, ting::IPAddress ip);

	void initialize();
	void update(long milli_time);

	void dedicatedServer(bool toggle) {	_dedicatedServer = toggle; }
	bool dedicatedServer(void) { return _dedicatedServer; }

	// Add new object to scene
	void addObject(WorldObject newObj);
	void addObjectPhys(WorldObject &newObj);

	void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	void loadLevel(const char * file);
};

#endif
