#include "Physics.h"
#include "../system/global.h"
#include "../scene/Tesselator.h"
#include "ActionHandler.h"

using namespace std;
//void Physics::emptyWorld()

void Physics::exitPhysics()
{
//  Physics::emptyWorld();
  delete groundBody;
  // remove the collision shapes
  for (int j = 0; j < collisionShapes.size(); j++)
  {
    btCollisionShape * shape = collisionShapes[j];
    collisionShapes[j] = 0;
    delete shape;
  }
  delete world;
  delete solver;
  delete broadphase;
  delete dispatcher;
  delete collConf;
  collisionShapes.clear();
}

void Physics::initPhysics()
{
  collConf = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collConf);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
           collConf);
  world->setGravity(btVector3(0,-10, 0));
  btCollisionShape * ground = new btBoxShape(btVector3(btScalar(global::map_width / 2.0),
          btScalar(GROUND_HEIGHT / 2.0), btScalar(global::map_height / 2.0)));
//  btCollisionShape * ground = new btStaticPlaneShape(btVector3(0,1,0), 50);
  btAlignedObjectArray<btCollisionShape *> collisionShapes;
  collisionShapes.push_back(ground);

  btTransform groundTrans;
  groundTrans.setIdentity();
  groundTrans.setOrigin(btVector3(0,-GROUND_HEIGHT / 2.0,0));
  btRigidBody::btRigidBodyConstructionInfo grbInfo(btScalar(0),
    new btDefaultMotionState(groundTrans), ground, btVector3(0,8,0));
  // Put the ground into the world
  groundBody = new btRigidBody(grbInfo);
  world->addRigidBody(groundBody);
  groundBody->setActivationState(ISLAND_SLEEPING);
  nextBlockNumber = 0;
  ActionHandler * action = new ActionHandler(this);
  world->addAction(action);
//  world->setInternalTickCallback(&tickCallback);
}

void Physics::update(int timeChange)
{
//  printf("Updating by: %d milliseconds.\n", timeChange);
  vector<WorldObject> changed;
  if (timeChange)
    world->stepSimulation(btScalar(timeChange / 1000.0), 1, btScalar(1 / 60.0));
  unsigned int i;
  int result;
  for (i = 0; i < physicsBodies.size(); i++)
  {
    result = physicsBodies[i]->update();
    if (result == 1)
    {
      changed.push_back(*(physicsBodies[i]->getWorldObject()));
    }
    if (result == 0)
    {
      //removeWorldObject(temp[i]->getWorldObject().getID());
    }
    else
    {
    }
  }
}

int Physics::isUniqueID(int id)
{
  unsigned int i;
  int result = 1;
  for (i = 0; result && i < physicsBodies.size(); i++)
  {
    if (physicsBodies[i]->getID() == id)
    {
      result = 0;
    }
  }
  return result;
}

/*void Physics::tickCallback(btDynamicsWorld *dWorld, btScalar timeChange)
{
//  for (int i = 0; i < physicsBodies.size(); i++)
  {
//    global::physicsBodies[i]->getWorldObject()->think();
  }
}*/

void Physics::addWorldObject(WorldObject *worldObject)
{
  
  if (isUniqueID(worldObject->getID()))
  {
    PhysicsBody * newBody = new PhysicsBody(worldObject);
    // stall slow objects

    if (worldObject->getVelocity().mag() < .01)
    {
      newBody->setActivationState(ISLAND_SLEEPING);
    }
    world->addRigidBody(newBody->getRigidBody());
    physicsBodies.push_back(newBody);
  }
  else
  {
    // TODO provide a reaction to a non-unique ID value.
  }
}

void Physics::emptyWorld()
{
#ifdef DEBUG
  printf("Function: Physics::emptyWorld() called, removing all PhysicsObjects\n");
#endif
  vector<PhysicsBody *> temp = physicsBodies;
  // blank out the viewed objects
  physicsBodies.clear();
  int i = temp.size();
  // iterate backwards through the existing PhysicsBodies, removing them all.
  while (i-- > 0)
  {
    world->removeRigidBody(temp[i]);
  }
  nextBlockNumber = 0;
}

bool Physics::removeWorldObject(int id)
{
  bool removed = false;
  for (unsigned int i = 0; !removed && i < physicsBodies.size(); i++)
  {
    if (physicsBodies[i]->getID() == id)
    {
      // Get rid of the associated PhysicsBody
      world->removeRigidBody(physicsBodies[i]->getRigidBody());
      physicsBodies.erase(physicsBodies.begin() + i);
      // Update the WorldObject associated with it
      update(0);
      removed = true;
    }
  }
  return removed;
}

vector<WorldObject *> Physics::getWorldObjects()
{
  vector<WorldObject *> result;
  for (unsigned int i = 0; i < physicsBodies.size(); i++)
  {
    result.push_back(physicsBodies[i]->getWorldObject());
  }
  return result;
}

void Physics::InsertNewBlock(ObjectType type, Vector position)
{
  WorldObject * newObj = new WorldObject(nextBlockNumber++, 0, type);
  newObj->setPosition(position);
  addWorldObject(newObj);
#ifdef DEBUG
  printf("Adding object at %s\n", position.str());
#endif
 }

vector<Vector> Physics::fileToBlockLocations(const char * fileName)
{
  Tesselator::dumpFileIntoPhysics(fileName);
  vector<Vector> result;
  return result;
}

// The current implementation only adds blocks to the world, no reinitilization.
int Physics::loadFromFile(const char * fileName)
{
  Physics::emptyWorld();
  vector<Vector> toPlace = Physics::fileToBlockLocations(fileName);
  return 1;
}
