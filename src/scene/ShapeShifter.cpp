#include "ShapeShifter.h"
#include "../system/global.h"

ShapeShifter::ShapeShifter() : WorldObject(0, 0, SHAPE_SHIFTER) {}
ShapeShifter::ShapeShifter(int a, int b, ObjectType c) : WorldObject(a, b, c) {}
void ShapeShifter::draw()
{
  float scale = global::factory->getBlueprint(type).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void ShapeShifter::think(int elapsedTime)
{
  // deal with collisions
}

