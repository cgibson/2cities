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
	//#define PRINTINFO(x,y) printf(x,y);
	//#define PRINTINFO(x,y,z) printf(x,y,z);
#else
/*
	#include "../graphics/graphics.h"
	#define PRINTINFO(x) gfx::hud.console.info(x);
	#define PRINTINFO(x,y) gfx::hud.console.info(x,y);
	#define PRINTINFO(x,y,z) gfx::hud.console.info(x,y,z);
	*/
	#define PRINTINFO(x) printf(x);
#endif

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

#include "Network.h"
using namespace net;

class NetworkSystem {
protected:
	int _playerID;
	unsigned int _currObjID;

	int _pktCountRecv; // Count of packets over a 250 ms period
	int _pktCountSent;
	int _pktPeriod;

	void updateObjectLocal(WorldObject *obj);
	void updateObjectVector(vector<WorldObject *> *objVec, WorldObject *objPtr);

	void removeObjectLocal(unsigned int worldObjectID);
	void removeObjectVector(vector<WorldObject *> *objVec, unsigned int worldObjectID);

	int SendPacket(NetworkPacket  pktPtr, ting::UDPSocket *socket, ting::IPAddress  destIP);
	int RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP);

	void buildBatchPacket(NetworkPacket *pkt, WorldObject objs[], unsigned int size);
	int  readBatchPacket (NetworkPacket *pkt, WorldObject objs[], unsigned int size);

public:
	NetworkSystem() {
		_pktCountRecv = 0;
		_pktCountSent = 0;
		_pktPeriod = 0;

		_playerID = 0;
	}
	~NetworkSystem() {
		closeSockets();
	};
	virtual void closeSockets() {};

	virtual void initialize() {};
	virtual void update(long elapsed) {};
	void updatePktData(long elapsed);

	int getPlayerID() { return _playerID; };

	virtual bool connectServer(const char * ip, unsigned int port) { return false; };
	virtual void disconnectServer() {};
	virtual PlayerColor getPlayerColor() { return (PlayerColor)_playerID; };

	virtual void dedicatedServer(bool toggle) {};
	virtual bool dedicatedServer(void) { return false; };

	virtual void sendMsg(char * msgStr) {};

	// Add new object to scene
	virtual void addObject(WorldObject newObj) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file) {};

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
