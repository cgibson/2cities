#include "BlackHole.h"
#include "../system/global.h"
#define BLACK_HOLE_FAR 35
#define BLACK_HOLE_NEAR 2

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
  float scale = global::factory->getBlueprint(type).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
//    glDisable(GL_LIGHTING);
//    glColor3f(0.0, 0.0, 0.0);
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
//    glEnable(GL_LIGHTING);
  glPopMatrix();
}

void BlackHole::think(int elapsedTime)
{
  // TODO add logic to cause black hole to move objects.
}
