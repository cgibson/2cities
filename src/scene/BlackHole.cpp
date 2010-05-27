#include "BlackHole.h"

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
