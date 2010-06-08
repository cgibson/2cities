#include "Physics.h"
#include "../system/global.h"
#include "../scene/Tesselator.h"
#include "ActionHandler.h"
#define NEXT_ID_PHYSICS_START 30000

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
  nextBlockNumber = NEXT_ID_PHYSICS_START;
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

  btTransform groundTrans;
  groundTrans.setIdentity();
  groundTrans.setOrigin(btVector3(0,-GROUND_HEIGHT / 2.0,0));
  btRigidBody::btRigidBodyConstructionInfo grbInfo(btScalar(0),
    new btDefaultMotionState(groundTrans), ground, btVector3(0,8,0));
  // Put the ground into the world
  groundBody = new btRigidBody(grbInfo);
  if (groundBody == NULL)
  {
	  printf("NULL groundBody found.\n");
	  exit(1);
  }
  world->addRigidBody(groundBody);
  groundBody->setActivationState(ISLAND_SLEEPING);
  ActionHandler * action = new ActionHandler(this);
  if (action == NULL)
  {
	  printf("NULL Action.\n");
	  exit(1);
  }
  world->addAction(action);
//  world->setInternalTickCallback(&tickCallback);
}

void Physics::update(int timeChange)
{
//  printf("Updating by: %d milliseconds.\n", timeChange);
  if (timeChange)
    world->stepSimulation(btScalar(timeChange / 1000.0), 1, btScalar(1 / 30.0));
  unsigned int i;
  for (i = 0; i < physicsBodies.size(); i++)
  {
    physicsBodies[i]->update();
    physicsBodies[i]->setGravity(btVector3(0, -10, 0));
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
    if (newBody->getRigidBody() != NULL)
    {
      world->addRigidBody(newBody->getRigidBody());
      physicsBodies.push_back(newBody);
    }
    else
    {
    	printf("Attempt to add NULL rigidBody.\n");
    	exit(1);
    }
  }
  else
  {
#ifdef DEBUG
    printf("Duplicate value found: %d\n", worldObject->getID());
#endif
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
  ghosts.clear();
  nextBlockNumber = NEXT_ID_PHYSICS_START;
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


int Physics::isNotWorldObject(btCollisionObject * toTest)
{
  int result = (toTest == groundBody);
  for (unsigned int i = 0; !result && i < ghosts.size(); i++)
  {
    result = (toTest == ghosts[i]);
  }
  return result;
}

void Physics::addGhost(btGhostObject * ghost)
{
  if (ghost == NULL)
  {
	printf("NULL ghostObject found.\n");
	exit(1);
  }
  ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  world->addCollisionObject(ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
  
  ghosts.push_back(ghost);
}

int Physics::removeGhost(btGhostObject * ghost)
{
  for (unsigned int i = 0; i < ghosts.size(); i++)
  {
    if (ghosts[i] == ghost)
    {
      ghosts.erase(ghosts.begin() + i);
      return 1;
    }
  }
  return 0;
}

btDiscreteDynamicsWorld * Physics::getWorld()
{
  return world;
}

PhysicsBody * Physics::getPBfromCO(btCollisionObject * in)
{
  for (unsigned int i = 0; i < physicsBodies.size(); i++)
    if (physicsBodies[i]->getRigidBody() == in)
      return physicsBodies[i];
  return NULL;
}

PhysicsBody * Physics::getPBfromID(int in)
{
  for (unsigned int i = 0; i < physicsBodies.size(); i++)
    if (physicsBodies[i]->getWorldObject()->getID() == (unsigned int)in)
      return physicsBodies[i];
  return NULL;
}

int Physics::insertWorldObject(WorldObject * worldObject)
{
  worldObject->setID(nextBlockNumber);
  networkManager->network->addObject(worldObject, worldObject->getID());
//  addWorldObject(worldObject);
  // TODO place the actual insertion code here.
#ifdef DEBUG
  printf("Physics is now inserting Object #%d.\n", nextBlockNumber);
#endif
  return nextBlockNumber++;
}
