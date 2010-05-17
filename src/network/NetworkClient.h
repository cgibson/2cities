#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"

#include "NetworkSystem.h"
using namespace Network;

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

#define MAX_PACKETS_PER_CYCLE 250

class NetworkClient : public NetworkSystem {
protected:
	ting::UDPSocket socket;
	ting::WaitSet *waitSet;
	ting::IPAddress serverIP;

	bool isConnected;

public:
	NetworkClient();
	~NetworkClient();

	virtual void initialize();
	virtual void update(long milli_time);

	virtual bool connectServer(const char * ip, unsigned int port);
	virtual void disconnectServer();
	virtual void sendMsg(char * msgStr);

	// Add new object to scene
	virtual void addObject(WorldObject newObj);

	virtual void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	void loadLevel(const char * file);
};

#endif
