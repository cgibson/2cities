#include "ClusterBomb.h"
#include "../system/global.h"

ClusterBomb::ClusterBomb() : WorldObject(0, 0, CLUSTER_BOMB) {}
ClusterBomb::ClusterBomb(int a, int b, ObjectType c) : WorldObject(a, b, c) {}
void ClusterBomb::draw()
{
  float scale = global::factory->getBlueprint(type).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void ClusterBomb::think(int elapsedTime)
{
  // deal with collisions
}

