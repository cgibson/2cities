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
	int serverTimeDelta;
	int lagCalc_StartTime;

public:
	NetworkClient();
	~NetworkClient();

	void initialize();
	void update(long milli_time);

	// Server Details
	virtual void closeSockets();
	//virtual void dedicatedServer(bool toggle) {};
	//virtual bool dedicatedServer(void) { return false; };

	// Connection Based Functions
	virtual bool connectServer(const char * ip, unsigned int port);
	virtual void disconnectServer();
	virtual int  checkLag(ting::UDPSocket *socket, ting::IPAddress ip);

	// Player Detail Functions
	//      int  getPlayerID() { return _playerID; };
	//virtual int  getPlayerScore(int playerID) { return 0; };
	//virtual PlayerColor getPlayerColor() { return (PlayerColor)_playerID; };

	virtual void sendPlayerReady(int readyFlag);

	// Communication
	virtual void sendMsg(char *msgStr);
	//virtual void recvMsg(NetworkPacket &pktPtr) {};

	// Add new object to scene
	virtual void addObject(WorldObject *objPtr);
	virtual void addObjectPhys(WorldObject *objPtr) { updateObjectLocal(objPtr); };
	//virtual void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	virtual void emptyWorld();
	virtual void loadLevel(const char * file);
	virtual void loadLevel(vector<WorldObject *> newObjs);
};

#endif
