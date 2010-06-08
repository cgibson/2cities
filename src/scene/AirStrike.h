#ifndef _AIR_STRIKE_H_
#define _AIR_STRIKE_H_

#include "WorldObject.h"


class AirStrike : public WorldObject
{
  private:
  int delay;
  int objectsLeft;
  
  public:
	AirStrike();
	AirStrike(int, int, ObjectType);
  
  void makeStrikeObject();
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

