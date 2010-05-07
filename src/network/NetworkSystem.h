#ifndef _NETWORKSYSTEM_H_
#define _NETWORKSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"
#include "../state/PhysObj.h" // TODO Remove with PhysObj Migration

#include "../physics/Physics.h" // TODO Remove with PhysObj Migration

#include "../system/global.h"
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

class NetworkSystem
{
protected:
	int _playerID;

public:
	Physics *physicsEngine; // TODO Remove with PhysObj Migration

	NetworkSystem() { _playerID = -1;	}
	~NetworkSystem() {};

	virtual void initialize() {};
	virtual void update(long milli_time) {};

	virtual bool connectServer(const char * ip, unsigned int port) { return false; };

	virtual int getPlayerID() { return _playerID; }

	// Add new object to scene
	virtual void addObject(WorldObject newObj) {};
	virtual void addObject(AmmoUnit newObj) {}; // TODO Remove with PhysObj Migration

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
