#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include "../physics/Physics.h"
#include "../state/PhysObj.h"
#include <vector>

class NetworkServer
{
    public:
	   NetworkServer();
       ~NetworkServer();

       void initialize();
       void update(long milli_time);
       void addObject(PhysObj newObj);
    protected:
       Physics physics;
       std::vector<PhysObj> objects;				// Later to be WorldObj type
};

#endif
