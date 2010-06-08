#ifndef _BLACKHOLE_H_
#define _BLACKHOLE_H_
#include "WorldObject.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

class BlackHole : public WorldObject
{
  private:
  int ttl;
  int id;
  btGhostObject * ghost;
  void setGhost();
  
  public:
	BlackHole();
	BlackHole(int, int, ObjectType);
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif
