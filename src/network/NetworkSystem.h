#ifndef _NETWORKSYSTEM_H_
#define _NETWORKSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../physics/Physics.h"

#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"

//#ifdef SERVER
	#define PRINTINFO(x) printf(x);
//#else
/*
	#include "../graphics/graphics.h"
	#define PRINTINFO(x) gfx::hud.console.info(x);
*/
//#endif

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

#include "Client.h"
#include "Network.h"
using namespace net;

class NetworkSystem {
protected:
	int myClientID;
	vector<Client *> clients;

	unsigned int nextNewObjID;
	int  timeToStateChange;
    bool timeToStateChangeSet;

	// Variables for rx/tx rates (counted over ~250ms period)
	int  _pktCountRecv;
	int  _pktCountSent;
	int  _pktPeriod;
    void updateRxTxData(long elapsed);

    // Server Function
	virtual void closeSockets() {};

	int lagCalc_StartTime;
	void sendServerLagReq(ting::UDPSocket *socketPtr, ting::IPAddress destIP);

	// Communication
	virtual void recvMsg(NetworkPacket &pkt);

	// Object Control
	virtual void addObjectPhys(WorldObject *objPtr) {};
	virtual void reqUpdateObj(unsigned int objID) {};

	// Helper functions for quick/sorted actions on std:vectors
	void updateObjectVector(vector<WorldObject *> *objVec, WorldObject *objPtr);
	void removeObjectVector(vector<WorldObject *> *objVec, unsigned int worldObjectID);
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
	void updOPlayerCamera();

public:
	NetworkSystem();
	~NetworkSystem();

	virtual void initialize() {};
	virtual void update(long elapsed) {};

	virtual Physics *getPhysicsPtr() { return NULL; }

	// Server Details
	virtual void dedicatedServer(bool toggle) {};
	virtual bool dedicatedServer(void) { return false; };

	// Connection Based Functions
	virtual bool serverConnect(const char * ip, unsigned int port) { return false; };
	virtual void serverDisconnect() {};
	virtual bool serverConnected() { return 0; };
	virtual int  getServerDelay() { return 0; };
	        int  getTimeToStateChange() { return global::elapsed_ms() - timeToStateChange; };

	// Player Detail Functions
	vector<Client *> getPlayers() { return clients; };
	        int  getMyPlayerID();
	        int  getMyClientID() { return myClientID; };
	        int  getPlayerReady(int playerID);
	        int  getPlayerScore(int playerID);
	        int  getPlayerDamage(int playerID);
	virtual PlayerColor getPlayerColor(int clientID) { return (PlayerColor)myClientID; };

	virtual void setPlayerReady(int readyFlag) {};
	virtual void setMyPlayerName(char *newName);
	virtual void setMyPlayerScore(int newScore) {};
	virtual void setMyPlayerDamage(int newDamage) {};

	// Communication
	virtual void sendMsg(char *msgStr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *objPtr) {};

	// Load a stored lvl
	virtual void emptyWorld();
	virtual void loadLevel(const char * file) {};
	virtual void loadLevel(vector<WorldObject *> newObjs);
};

#endif
