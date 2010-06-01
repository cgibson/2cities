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

	// Variables for rx/tx rates (counted over ~250ms period)
	int _pktCountRecv;
	int _pktCountSent;
	int _pktPeriod;

	// Helper functions for quick/sorted actions on std:vectors
	void updateObjectVector(vector<WorldObject *> *objVec, WorldObject *objPtr);
	void removeObjectVector(vector<WorldObject *> *objVec, unsigned int worldObjectID);

	// Helper functions to specifically act on currState->objects vector
	void updateObjectLocal(WorldObject *obj);
	void removeObjectLocal(unsigned int worldObjectID);


	int SendPacket(NetworkPacket &pktPtr, ting::UDPSocket *socket, ting::IPAddress  destIP);
	int RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP);

	void buildBatchPacket(NetworkPacket *pkt, WorldObject *objs[], unsigned int size);

	void decodeObjectSend(NetworkPacket &pkt, long interpValue);

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
	void updateRxTxData(long elapsed);

	int getPlayerID() { return _playerID; };

	virtual bool connectServer(const char * ip, unsigned int port) { return false; };
	virtual void disconnectServer() {};

	virtual int  checkLag(ting::UDPSocket *socket, ting::IPAddress ip) { return 0; };

	virtual PlayerColor getPlayerColor() { return (PlayerColor)_playerID; };

	virtual void dedicatedServer(bool toggle) {};
	virtual bool dedicatedServer(void) { return false; };

	virtual void sendMsg(char * msgStr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *objPtr) {};
	virtual void addObjectPhys(WorldObject *objPtr) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file) {};
	virtual void loadLevel(vector<WorldObject *> newObjs) {};

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
