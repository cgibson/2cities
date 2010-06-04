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

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

class NetworkObjectState; // See Bottom

#define CLIENT_TIMEOUT 5000

class NetworkServer : public NetworkSystem {
protected:
	Physics physicsEngine;

	ting::UDPSocket serverSocket;
	ting::WaitSet *_waitSet;

	vector<WorldObject *> _serverObjs;

	bool _dedicatedServer;
	unsigned int _sendObjNext;

	virtual void closeSockets();

	//virtual void recvMsg(NetworkPacket &pktPtr) {};

	virtual void addObjectPhys(WorldObject *objPtr);
	//virtual void reqUpdateObj(unsigned int objID) {};

	void networkIncoming(long &elapsed);
	void networkIncomingGeneral(long &elapsed);
	void networkIncomingPlayers(int p, long &elapsed);
	void networkOutgoing(long &elapsed);

	void checkStateChange();
	void checkClientTimeout();
	void playerDisconnect(int clientID);

public:
	NetworkServer();
	~NetworkServer();

	void initialize();
	void update(long milli_time);

	// Server Details
	virtual void dedicatedServer(bool toggle) {	_dedicatedServer = toggle; }
	virtual bool dedicatedServer(void) { return _dedicatedServer; }

	// Connection Based Functions
	virtual bool serverConnect(const char * ip, unsigned int port) { return true; };
	//virtual void serverDisconnect() {};
	virtual bool serverConnected() { return 1; };
	virtual int  getServerDelay() { return 0; };

	// Player Detail Functions
	//virtual int  getPlayerScore(int playerID) { return 0; };
	virtual PlayerColor getPlayerColor() { return (PlayerColor)myClientID; };

	//virtual void sendPlayerReady(int readyFlag) {};

	// Communication
	//virtual void sendMsg(char *msgStr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *objPtr);

	// Load a stored lvl
	virtual void emptyWorld();
	virtual void loadLevel(const char * file);
	//virtual void loadLevel(vector<WorldObject *> newObjs) {};
};

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

#endif
