#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include "../physics/Physics.h"
#include "../scene/WorldObject.h"
#include "../system/global.h"
#include <vector>
#include <map>

class NetworkServer
{
    public:
	   NetworkServer();
       ~NetworkServer();

       void initialize();
       void update(long milli_time);
       void addObject(WorldObject newObj);
    protected:
       Physics physics;
       std::vector<WorldObject *> objects;
};

#endif
