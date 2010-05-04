#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"
#include "../state/CarnageState.h"
#include "../system/enum.h"
#include "../system/global.h"

#include "../physics/Physics.h"

using namespace enumeration;

enum commType { AddObject, ReqUpdateObject, ReqUpdateState };

class NetworkClient
{
	protected:
		Physics physicsEngine; // Temp Include

	    unsigned int serverIP; // Type Likely to Change
	    unsigned int serverPort;

	    int playerID;

	    void sendObject(enum E_WorldObjType newObjType, WorldObject newObj);

    public:
	   NetworkClient();
       ~NetworkClient();

       void initialize();
       void update(long milli_time);

       void connectServer(/*ip*/);

       // Create/Send a new Object to Server for propagation/Physics
       void addObject(enum E_WorldObjType newObjType, WorldObject newObj);

       void reqUpdateObj(unsigned int objID);
       void reqUpdateState(enum E_STATE);
};

#endif
