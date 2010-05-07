#ifndef _NETWORKPRIVATE_H_
#define _NETWORKPRIVATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "NetworkSystem.h"

#include "../scene/WorldObject.h"
#include "../state/PhysObj.h" // TODO Remove with PhysObj Migration

#include "../physics/Physics.h"

#include "../system/global.h"
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

class NetworkPrivate : public NetworkSystem
{
protected:

public:
	Physics *physicsEngine; // TODO Move to protected with PhysObj Migration

	NetworkPrivate();
	~NetworkPrivate();

	virtual void initialize();
	virtual void update(long milli_time);

	// Add new object to scene
	virtual void addObject(WorldObject newObj);
	virtual void addObject(AmmoUnit newObj); // TODO Remove with PhysObj Migration
};

#endif
