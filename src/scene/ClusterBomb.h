#ifndef _CLUSTER_BOMB_H_
#define _CLUSTER_BOMB_H_

#include "WorldObject.h"
#define FUSE_LENGTH 1000
#define BOMB_PIECES 10
#define FUSE_TRIGGER 10.0f

class ClusterBomb : public WorldObject
{
  private:
  int fuse;
  void explode();
  public:
	ClusterBomb();
	ClusterBomb(int, int, ObjectType);
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

