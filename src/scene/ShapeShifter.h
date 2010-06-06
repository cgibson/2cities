#ifndef _SHAPE_SHIFTER_H_
#define _SHAPE_SHIFTER_H_

#include "WorldObject.h"


class ShapeShifter : public WorldObject
{
  public:
	ShapeShifter();
	ShapeShifter(int, int, ObjectType);
  
  virtual void draw();
  virtual void think(int elapsedTime);
};

#endif

