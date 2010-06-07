#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"

#include "NetworkSystem.h"
using namespace net;

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

#ifndef MAX_RECV_PACKETS_PER_CYCLE
	#define MAX_RECV_PACKETS_PER_CYCLE 25
#endif

class NetworkClient : public NetworkSystem {
protected:
	ting::UDPSocket socket;
	ting::WaitSet *waitSet;
	ting::IPAddress serverIP;

	bool isConnected;
	unsigned int serverDelay;
	int  serverClockDelta;

	virtual void closeSockets();

	virtual void addObjectPhys(WorldObject *objPtr) { updateObjectLocal(objPtr); };

	void recvStatusUpdate(NetworkPacket &pkt);
	void networkIncoming(long &elapsed);

public:
	NetworkClient();
	~NetworkClient();

	void initialize() {};
	void update(long milli_time);

	// Server Details
	virtual void dedicatedServer(bool toggle) {};
	virtual bool dedicatedServer(void) { return false; };

	// Connection Based Functions
	virtual bool serverConnect(const char * ip, unsigned int port);
	virtual void serverDisconnect();
	virtual bool serverConnected() { return isConnected; };
	virtual int  getServerDelay();

	// Player Detail Functions
	virtual void setPlayerReady(int readyFlag);
	virtual void setMyPlayerName(char *newName);
	virtual void setMyPlayerScore(int newScore);
	virtual void setMyPlayerDamage(int newDamage);

	// Communication
	virtual void sendMsg(char *msgStr);

	// Add new object to scene
	virtual void addObject(WorldObject *ObjPtr, int newID = -1);

	// Load a stored lvl
	virtual void emptyWorld();
	virtual void loadLevel(const char * file);
	virtual void loadLevel(vector<WorldObject *> newObjs);
};

#endif
