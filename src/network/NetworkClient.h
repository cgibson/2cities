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
	void closeSockets();

	void initialize();
	void update(long milli_time);

	bool connectServer(const char * ip, unsigned int port);
	void disconnectServer();
	int  checkLag(ting::UDPSocket *socket, ting::IPAddress ip);

	void sendMsg(char * msgStr);

	// Add new object to scene
	void addObject(WorldObject *newObj);
	void addObjectPhys(WorldObject *ObjPtr) { updateObjectLocal(ObjPtr); }

	void reqUpdateObj(unsigned int objID) {};

	// Load a stored lvl
	void loadLevel(const char * file);
};

#endif
