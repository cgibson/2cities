#include "AirStrike.h"
#include "../system/global.h"
#include "stdlib.h"
#define AIR_STRIKE_DELAY 100
#define STRIKE_COUNT 10
#define AIR_STRIKE_FUSE_TRIGGER 1.0f


AirStrike::AirStrike() : WorldObject(0, 0, AIR_STRIKE) {delay = 0;objectsLeft = STRIKE_COUNT;}
AirStrike::AirStrike(int a, int b, ObjectType c) : WorldObject(a, b, c) {delay = 0;objectsLeft = STRIKE_COUNT;}
void AirStrike::draw()
{
  float scale = global::factory->getBlueprint(type).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void AirStrike::makeStrikeObject()
{
  WorldObject * wo;
  float angle = 6.28318 / 2;
  Vector a = Vector(1, 0, 0);
  Vector b = Vector(0, 0, 1);
  float mag = global::factory->getBlueprint(getType()).getSize().mag();
  Vector off;
  wo = new WorldObject(0, 0, DUMMY_CONE);
  off = a * mag * cos(angle) + b * mag * sin(angle);
  Vector newPos = position + Vector(0, 50, 0) + a * (rand() % 10 - 4.5) + b * (rand() % 10 - 4.5);
  printf("position: %s", newPos.str());
  wo->setPosition(newPos);
  wo->setVelocity(Vector(0, -80, 0));
  wo->setOrientation(Quaternion(1, 0, 0, 3.14159 / 2));
  networkManager->network->getPhysicsPtr()->insertWorldObject(wo);
}

void AirStrike::think(int elapsedTime)
{
  if (objectsLeft > 0 && velocity.mag() < AIR_STRIKE_FUSE_TRIGGER)
  {
    delay -= elapsedTime;
    if (delay <= 0)
    {
      makeStrikeObject();
      delay = AIR_STRIKE_DELAY;
      objectsLeft--;
    }
  }
}

