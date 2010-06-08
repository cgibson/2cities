#include "Shotgun.h"
#include "../system/global.h"

Shotgun::Shotgun() : WorldObject(0, 0, SHOTGUN) {exploded = 0;}
Shotgun::Shotgun(int a, int b, ObjectType c) : WorldObject(a, b, c) {exploded = 0;}
void Shotgun::draw()
{
  float scale = global::factory->getBlueprint(getType()).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void Shotgun::explode()
{
  WorldObject * wo;
  float angle = 6.28318 / SHOTGUN_CHUNKS;
  Vector a = velocity.cross(Vector(0, 1, 0));
  Vector b = a.cross(velocity);
  a.norm();
  b.norm();
  float mag = global::factory->getBlueprint(type).getSize().mag();
  Vector off;
  for (int i = 0; i < SHOTGUN_CHUNKS; i++)
  {
    wo = new WorldObject(0, 0, DUMMY_SPHERE);
    off = a * mag * sin(i * angle) + b * mag * cos(i * angle);
//    printf("mag: %f a:%s\n", mag, a.str());
    wo->setPosition(position + off);
    wo->setVelocity(velocity + off);
    networkManager->network->getPhysicsPtr()->insertWorldObject(wo);
  }
}

void Shotgun::think(int elapsedTime)
{
  if (!exploded)
  {
#ifdef DEBUG
//    printf("Shotgun shot is thinking: \"Explode!\"\n");
#endif
    explode();
    exploded = 1;
    // TODO remove the first piece, this is optional for Shotgun though.
    setToCull();
  }
}

