#ifndef _TESSELATOR_H_
#define _TESSELATOR_H_

#include <stdio.h>
#include "../system/enum.h"
#include "WorldObject.h"
#include "../physics/Physics.h"

#define TESS_MIN_X  1
#define TESS_MIN_Y  2
#define TESS_MIN_Z  4
#define TESS_MAX_X  8
#define TESS_MAX_Y 16
#define TESS_MAX_Z 32
#define TESS_BLOCKSIZE 5

namespace Tesselator
{
  extern void placeBlock(ObjectType type, Vector lowerBound);
  extern void placeSimpleSegment(Vector spot, int bits);
  extern void placeSegment(Vector spot, Tesselation type, int bits);
  extern void placeBuilding(Vector lowerBound, Vector upperBound,
        Tesselation type);
  extern void dumpFileIntoPhysics(const char * fileName);
}

#endif
