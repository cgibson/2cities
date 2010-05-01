#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include "../state/PhysObj.h"

class NetworkClient
{
    public:
	   NetworkClient();
       ~NetworkClient();

       void initialize();
       void update(long milli_time);
       void addObject(PhysObj newObj);
};

#endif
