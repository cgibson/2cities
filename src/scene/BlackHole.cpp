#include "BlackHole.h"
#include "../system/global.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#define BLACK_HOLE_FAR 17
#define BLACK_HOLE_NEAR 3
#define BLACK_HOLE_TIMEOUT_MS 10000

BlackHole::BlackHole() :
  WorldObject(0, 0, BLACK_HOLE)
{
  setGhost();
}

BlackHole::BlackHole(int a, int b, ObjectType c) :
  WorldObject(a, b, c)
{
  setGhost();
}

void BlackHole::draw()
{
  float scale = global::factory->getBlueprint(getType()).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
//    glDisable(GL_LIGHTING);
//    glColor3f(0.0, 0.0, 0.0);
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
//    glEnable(GL_LIGHTING);
  glPopMatrix();
}

void BlackHole::setGhost()
{
  ttl = BLACK_HOLE_TIMEOUT_MS;
  ghost = NULL;
  if (networkManager->network->getPhysicsPtr() != NULL)
  {
    id = networkManager->network->getPhysicsPtr()->getNextBlockNumber();
    ghost = new btGhostObject();
    ghost->setCollisionShape(global::factory->getShape(BLACK_HOLE_GHOST));
    networkManager->network->getPhysicsPtr()->addGhost(ghost);
  }
}

/*vector::<PhysicsBody *> getTooClose()
{
  vector<PhysicsBody *> result;
  Physics * physics = networkManager->network->getPhysicsPtr();
  if (physics == NULL)
  {
    return result;
  }
  WorldObject * wo2;
  float dist;
  for (unsigned int i = 0; i < network->size(); i++)
  {
    wo2 = physics->physicsBodies[i]->getWorldObject()
    dist = (wo2.getPosition() - position).mag()
    if (dist < BLACK_HOLE_FAR)
    {
      result.push_back(physicsBodies[i]);
    }
  }
}*/


void BlackHole::think(int elapsedTime)
{
  if (ghost == NULL)return;
//  printf("%d OverlappingObjects found.\n", ghost->getNumOverlappingObjects());
  PhysicsBody * pb = NULL;
  Physics * physics = networkManager->network->getPhysicsPtr();
  
  for (int i = 0; i < ghost->getNumOverlappingObjects(); i++)
  {
    pb = physics->getPBfromCO(ghost->getOverlappingObject(i));
    if (pb != NULL)
    {
      if (pb->getWorldObject()->getType() != BLACK_HOLE)
        pb->drawnByBlackHoleAt(position, 150 * (elapsedTime / 1000.0f),
                               BLACK_HOLE_FAR,
                               BLACK_HOLE_NEAR);
    }
  }
  
  // update ghost location.
  btTransform trans = ghost->getWorldTransform();
  trans.setOrigin(PhysicsBody::VtobtV3(position));
  ghost->setWorldTransform(trans);
  ttl -= elapsedTime;
  // check for time out
  if (ttl <= 0)
  {
    pb = physics->getPBfromID(id);
    physics->removeGhost(ghost);
    ghost = NULL;
    pb->setToCull();
  }
}
