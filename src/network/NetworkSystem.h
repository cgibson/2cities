#ifndef _NETWORKSYSTEM_H_
#define _NETWORKSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../system/global.h"
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

class NetworkSystem
{
protected:
	int _playerID;

public:
	NetworkSystem() { _playerID = -1;	}
	~NetworkSystem() {};

	virtual void initialize() {};
	virtual void update(long milli_time) {};

	virtual bool connectServer(const char * ip, unsigned int port) { return false; };

	virtual int getPlayerID() { return _playerID; }

	// Add new object to scene
	virtual void addObject(WorldObject newObj) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file) {};

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
