#include "BlackHole.h"

BlackHole::BlackHole() :
  WorldObject(0, 0, BLACK_HOLE)
{
}

BlackHole::BlackHole(int a, int b, ObjectType c) :
  WorldObject(a, b, c)
{
}

void BlackHole::draw()
{
  glutSolidTeapot(10);//global::factory->getBlueprint(c).getSize().mag());
}

void BlackHole::think(int elapsedTime)
{
}
