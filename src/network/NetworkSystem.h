#ifndef _NETWORKSYSTEM_H_
#define _NETWORKSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"

#ifdef SERVER
	#define PRINTINFO(x) printf(x);
#else
	//#include "../graphics/graphics.h"
	//#define PRINTINFO(x) gfx::hud.console.info(x);
	#define PRINTINFO(x) printf(x);
#endif

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

#include "Network.h"
using namespace Network;

class NetworkSystem {
protected:
	int _playerID;
	unsigned int _currObjID;

	void updateLocalObject(WorldObject *obj);

	int SendPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *destIP);
	int RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP);

public:
	NetworkSystem() {}
	~NetworkSystem() {};

	virtual void initialize() {};
	virtual void update(long milli_time) {};

	int getPlayerID() { return _playerID; };

	virtual bool connectServer(const char * ip, unsigned int port) { return false; };
	virtual void sendMsg(char * msgStr) {};

	// Add new object to scene
	virtual void addObject(WorldObject newObj) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file) {};

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
