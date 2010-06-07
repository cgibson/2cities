#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"
#include "WorldObjectState.h"

#include "../physics/Physics.h"

#include "NetworkSystem.h"
#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

class NetworkObjectState; // See Bottom

#define CLIENT_TIMEOUT 5000

class NetworkServer : public NetworkSystem {
protected:
	Physics physicsEngine;

	ting::UDPSocket serverSocket;
	ting::WaitSet *_waitSet;

	vector<WorldObjectState> _serverObjs;

	bool _dedicatedServer;
	unsigned int _sendObjNext;

	virtual void closeSockets();

	virtual void recvMsg(NetworkPacket &pkt);

	virtual void addObjectPhys(WorldObject *objPtr);
	//virtual void reqUpdateObj(unsigned int objID) {};

	void networkIncoming(long &elapsed);
	void networkIncomingGeneral(long &elapsed);
	void networkIncomingPlayers(int p, long &elapsed);
	void networkOutgoing(long &elapsed);

	void checkStateChange();
	void checkClientTimeout();
	int  checkWinCondition();

	void updatePlayerDetails(); // Damage/Score

	void playerDisconnect(int clientID);

	void sendStatusUpdates();

public:
	NetworkServer();
	~NetworkServer();

	void initialize() {};
	void update(long milli_time);

	virtual Physics *getPhysicsPtr() { return &physicsEngine; }

	// Server Details
	virtual void dedicatedServer(bool toggle) {	_dedicatedServer = toggle; }
	virtual bool dedicatedServer(void) { return _dedicatedServer; }

	// Connection Based Functions
	virtual bool serverConnect(const char * ip, unsigned int port) { return true; };
	virtual bool serverConnected() { return 1; };
	virtual int  getServerDelay() { return 0; };

	// Player Details
	virtual void setPlayerReady(int readyFlag);

	// Communication
	virtual void sendMsg(char *msgStr);

	// Add new object to scene
	virtual void addObject(WorldObject *ObjPtr, int newID = -1);

	// Load a stored lvl
	virtual void emptyWorld();
	virtual void loadLevel(const char * file);
};

#endif
