#ifndef _NETWORKPRIVATE_H_
#define _NETWORKPRIVATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "NetworkSystem.h"

#include "../scene/WorldObject.h"

#include "../physics/Physics.h"

#include "../system/global.h"
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

class NetworkPrivate : public NetworkSystem
{
protected:
	Physics *physicsEngine;

public:
	NetworkPrivate();
	~NetworkPrivate();

	virtual void initialize();
	virtual void update(long milli_time);

	// Add new object to scene
	virtual void addObject(WorldObject newObj);

	// Load a stored lvl
	virtual void loadLevel(const char * file);
};

#endif
