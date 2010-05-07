#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../physics/Physics.h"

#include "../system/global.h"
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

class NetworkServer : public NetworkSystem
{
protected:
	Physics *physicsEngine;

public:
	NetworkServer();
	~NetworkServer();

	virtual void initialize();
	virtual void update(long milli_time);

	virtual int getPlayerID() { return _playerID; }

	// Add new object to scene
	virtual void addObject(WorldObject newObj);

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
