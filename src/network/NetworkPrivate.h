#ifndef _NETWORKPRIVATE_H_
#define _NETWORKPRIVATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "NetworkSystem.h"

#include "../scene/WorldObject.h"

#include "../physics/Physics.h"

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

class NetworkPrivate : public NetworkSystem {
protected:
	Physics physicsEngine;

public:
	NetworkPrivate();
	~NetworkPrivate();

	virtual void initialize() {};
	virtual void update(long milli_time);

	virtual Physics *getPhysicsPtr() { return &physicsEngine; }

	// Server Details

	// Connection Based Functions
	virtual bool serverConnected() { return 1; };

	// Player Detail Functions
	virtual void setPlayerReady(int readyFlag);

	// Communication
	//virtual void sendMsg(char *msgStr) {};
	//virtual void recvMsg(NetworkPacket &pktPtr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *ObjPtr, int newID = -1);
	virtual void addObjectPhys(WorldObject *objPtr);

	// Load a stored lvl
	virtual void emptyWorld();
	virtual void loadLevel(const char * file);
};

#endif
