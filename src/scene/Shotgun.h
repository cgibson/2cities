#ifndef _SHOTGUN_H_
#define _SHOTGUN_H_

#define SHOTGUN_CHUNKS 5
#include "WorldObject.h"


class Shotgun : public WorldObject
{
  private:
    int exploded;
  public:
	Shotgun();
	Shotgun(int, int, ObjectType);
  void explode();
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

