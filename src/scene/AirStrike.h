#ifndef _AIR_STRIKE_H_
#define _AIR_STRIKE_H_

#include "WorldObject.h"


class AirStrike : public WorldObject
{
  public:
	AirStrike();
	AirStrike(int, int, ObjectType);
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

