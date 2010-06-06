#include "AirStrike.h"
#include "../system/global.h"

AirStrike::AirStrike() : WorldObject(0, 0, AIR_STRIKE) {}
AirStrike::AirStrike(int a, int b, ObjectType c) : WorldObject(a, b, c) {}
void AirStrike::draw()
{
  float scale = global::factory->getBlueprint(type).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void AirStrike::think(int elapsedTime)
{
  // deal with collisions
}

