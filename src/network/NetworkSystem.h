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
    void updateRxTxData(long elapsed);

	// Helper functions for quick/sorted actions on std:vectors
	void updateObjectVector(vector<WorldObject *> *objVec, WorldObject *objPtr);
	void removeObjectVector(vector<WorldObject *> *objVec, unsigned int worldObjectID);

	// Helper functions to specifically act on currState->objects vector
	void updateObjectLocal(WorldObject *obj);
	void removeObjectLocal(unsigned int worldObjectID);

	// Helper functions to abstract packet filling
	int SendPacket(NetworkPacket &pkt   , ting::UDPSocket *socket, ting::IPAddress  destIP);
	int RecvPacket(NetworkPacket *pktPtr, ting::UDPSocket *socket, ting::IPAddress *srcIP);

	// Object Packaging Helpers
	void buildBatchPacket(NetworkPacket *pkt, WorldObject *objs[], unsigned int size);
	void decodeObjectSend(NetworkPacket &pkt, long interpValue);

	// Player Status Updaters
	int  makePlayerCamera(Vector  camPos, Vector  camView, unsigned char *bufPtr);
	void sendPlayerCamera(Vector  camPos, Vector  camView, ting::UDPSocket *socketPtr, ting::IPAddress destIP);
	void recvPlayerCamera(Vector &camPos, Vector &camView, unsigned char *bufPtr);

public:
	NetworkSystem() { _pktCountRecv = _pktCountSent = _pktPeriod = _playerID = 0; };
	~NetworkSystem() { closeSockets(); };

	virtual void initialize() {};
	virtual void update(long elapsed) {};

	// Server Details
	virtual void closeSockets() {};
	virtual void dedicatedServer(bool toggle) {};
	virtual bool dedicatedServer(void) { return false; };

	// Connection Based Functions
	virtual bool connectServer(const char * ip, unsigned int port) { return false; };
	virtual void disconnectServer() {};
	virtual int  checkLag(ting::UDPSocket *socket, ting::IPAddress ip) { return 0; };

	// Player Detail Functions
	        int  getPlayerID() { return _playerID; };
	virtual int  getPlayerScore(int playerID) { return 0; };
	virtual PlayerColor getPlayerColor() { return (PlayerColor)_playerID; };

	virtual void sendPlayerReady(int readyFlag) {};

	// Communication
	virtual void sendMsg(char *msgStr) {};
	virtual void recvMsg(NetworkPacket &pktPtr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *objPtr) {};
	virtual void addObjectPhys(WorldObject *objPtr) {};
	virtual void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file) {};
	virtual void loadLevel(vector<WorldObject *> newObjs) {};
};

#endif
