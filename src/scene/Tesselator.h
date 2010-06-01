#ifndef _TESSELATOR_H_
#define _TESSELATOR_H_

#include <stdio.h>
#include <vector>
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
  extern WorldObject * placeBlock(ObjectType type, Vector lowerBound);
  extern std::vector<WorldObject *> placeSolidSegment(Vector spot, int bits);
  extern std::vector<WorldObject *> placeStoneHengeSegment(Vector spot, int bits);
  extern std::vector<WorldObject *> placeSimpleSegment(Vector spot, int bits);
  extern std::vector<WorldObject *> placeSegment(Vector spot, Tesselation type, int bits);
  extern std::vector<WorldObject *> placeBuilding(Vector lowerBound, Vector upperBound,
        Tesselation type);
  extern void dumpFileIntoPhysics(const char * fileName);
}

#endif
