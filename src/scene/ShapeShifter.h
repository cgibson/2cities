#ifndef _SHAPE_SHIFTER_H_
#define _SHAPE_SHIFTER_H_

#include "WorldObject.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"


class ShapeShifter : public WorldObject
{
  private:
  int numberShifted;
  int id;
  btGhostObject * ghost;
  void setGhost();
    
  public:
	ShapeShifter();
	ShapeShifter(int, int, ObjectType);
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

