#include "ClusterBomb.h"
#include "../system/global.h"

ClusterBomb::ClusterBomb() : WorldObject(0, 0, CLUSTER_BOMB) {fuse = FUSE_LENGTH;}
ClusterBomb::ClusterBomb(int a, int b, ObjectType c) : WorldObject(a, b, c) {fuse = FUSE_LENGTH;}
void ClusterBomb::draw()
{
  float scale = global::factory->getBlueprint(type).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void ClusterBomb::explode()
{
#ifdef DEBUG
//  printf("ClusterBomb just exploded!\n");
#endif
  WorldObject * wo;
  float angle = 6.28318 / BOMB_PIECES;
  Vector a = Vector(1, 0, 0);
  Vector b = Vector(0, 0, 1);
  Vector c = Vector(0, .1, 0);
  float mag = global::factory->getBlueprint(type).getSize().mag() / 4;
  Vector off;
  for (int i = 0; i < BOMB_PIECES; i++)
  {
    wo = new WorldObject(0, 0, DUMMY_SPHERE);
    off = a * mag * sin(i * angle) + b * mag * cos(i * angle) + c;
    wo->setPosition(position + off);
    wo->setVelocity(velocity + off);
    networkManager->network->getPhysicsPtr()->insertWorldObject(wo);
    wo = new WorldObject(0, 0, DUMMY_SPHERE);
    off = a * mag * cos(i * angle) + b * mag * sin(i * angle) - c;
    wo->setPosition(position + off);
    wo->setVelocity(velocity + off);
    networkManager->network->getPhysicsPtr()->insertWorldObject(wo);
  }
}



void ClusterBomb::think(int elapsedTime)
{
  if (velocity.mag() < FUSE_TRIGGER && fuse > 0)
  {
    fuse -= elapsedTime;
    if (fuse <= 0)
    {
      explode();
    }
  }
}

