#include "ShapeShifter.h"
#include "../system/global.h"

ShapeShifter::ShapeShifter() : WorldObject(0, 0, SHAPE_SHIFTER) {setGhost();}
ShapeShifter::ShapeShifter(int a, int b, ObjectType c) : WorldObject(a, b, c) {setGhost();}
void ShapeShifter::draw()
{
  float scale = global::factory->getBlueprint(getType()).getSize().mag();
  glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale, scale, scale);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

void ShapeShifter::setGhost()
{
  numberShifted = 0;
  ghost = NULL;
  if (networkManager->network->getPhysicsPtr() != NULL)
  {
    id = networkManager->network->getPhysicsPtr()->getNextBlockNumber();
    ghost = new btGhostObject();
    ghost->setCollisionShape(global::factory->getShape(SHAPE_SHIFTER_GHOST));
    networkManager->network->getPhysicsPtr()->addGhost(ghost);
  }
}

void ShapeShifter::think(int elapsedTime)
{
  if (numberShifted > 0 || ghost == NULL) return;
  PhysicsBody * pb = NULL;
  Physics * physics = networkManager->network->getPhysicsPtr();
  vector<WorldObject *> newStuff;
  printf("Ghost->getNumOverlappingObjects() == %d\n", ghost->getNumOverlappingObjects());
  for (int i = 0; i < ghost->getNumOverlappingObjects(); i++)
  {
    pb = physics->getPBfromCO(ghost->getOverlappingObject(i));
    if (pb != NULL && pb->getWorldObject()->getType() != SHAPE_SHIFTER)
    {
      WorldObject * wo = new WorldObject(0, 0, DUMMY_SPHERE);
      WorldObject * old = pb->getWorldObject();
      wo->setPosition(old->getPosition());
      wo->setOrientation(old->getOrientation());
      wo->setVelocity(old->getVelocity());
      // Place the new world object where the old one was.
      newStuff.push_back(wo);
      // Now remove the old one
      pb->setToCull();
      numberShifted++;
    }
  }
  for (unsigned int j = 0; j < newStuff.size(); j++)
  {
    physics->insertWorldObject(newStuff[j]);
  }
  // update ghost location.
	btTransform trans = ghost->getWorldTransform();
	trans.setOrigin(PhysicsBody::VtobtV3(position));
	ghost->setWorldTransform(trans);
  // after at least one thing has been shifted, remove the ammo.
  if (numberShifted > 0)
  {
    pb = physics->getPBfromID(id);
    physics->removeGhost(ghost);
    ghost = NULL;
    pb->setToCull();
  }
}

