#ifndef _BLACKHOLE_H_
#define _BLACKHOLE_H_
#include "WorldObject.h"

class BlackHole : public WorldObject
{
//  private btGhostBody *
  public:
	BlackHole();
	BlackHole(int, int, ObjectType);
  
  
  
//  btGhostBody * initBlackHole();
  
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif
