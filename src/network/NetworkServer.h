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

class NetworkServer : public NetworkSystem
{
protected:
	Physics *physicsEngine;
	vector<Player *> players;

	ting::UDPSocket *incomingSock;
	ting::WaitSet *waitSet;
	int lastGroup;

public:
	NetworkServer();
	~NetworkServer();

	virtual void initialize();
	virtual void update(long milli_time);

	// Add new object to scene
	virtual void addObject(WorldObject newObj);

	virtual void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file);
};

#endif
