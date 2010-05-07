#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../system/global.h"
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

class NetworkClient : public NetworkSystem
{
protected:

public:
	NetworkClient();
	~NetworkClient();

	virtual void initialize();
	virtual void update(long milli_time);

	virtual void connectServer(const char * ip, unsigned int port);

	virtual int getPlayerID() { return _playerID; }

	// Add new object to scene
	virtual void addObject(WorldObject newObj);

	virtual void reqUpdateObj(unsigned int objID) {};
};

#endif
