#ifndef _CLUSTER_BOMB_H_
#define _CLUSTER_BOMB_H_

#include "WorldObject.h"


class ClusterBomb : public WorldObject
{
  public:
	ClusterBomb();
	ClusterBomb(int, int, ObjectType);
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

