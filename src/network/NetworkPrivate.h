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

	virtual void initialize();
	virtual void update(long milli_time);

	// Server Details
	//virtual void closeSockets() {};
	//virtual void dedicatedServer(bool toggle) {};
	//virtual bool dedicatedServer(void) { return false; };

	// Connection Based Functions
	//virtual bool connectServer(const char * ip, unsigned int port) { return false; };
	//virtual void disconnectServer() {};
	//virtual int  checkLag(ting::UDPSocket *socket, ting::IPAddress ip) { return 0; };

	// Player Detail Functions
	//      int  getPlayerID() { return _playerID; };
	//virtual int  getPlayerScore(int playerID) { return 0; };
	//virtual PlayerColor getPlayerColor() { return (PlayerColor)_playerID; };
	//virtual void sendPlayerCamera() {};
	//virtual void sendPlayerReady(int readyFlag) {};

	// Communication
	//virtual void sendMsg(char *msgStr) {};
	//virtual void recvMsg(NetworkPacket &pktPtr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *objPtr);
	//virtual void addObjectPhys(WorldObject *objPtr) {};
	//virtual void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file);
	//virtual void loadLevel(vector<WorldObject *> newObjs) {};
};

#endif
