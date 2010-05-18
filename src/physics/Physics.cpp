#include "Physics.h"
#include "../system/global.h"

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
  btCollisionShape * ground = new btBoxShape(btVector3(btScalar(100.),
          btScalar(GROUND_HEIGHT / 2.0), btScalar(100.)));
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
  bldgShape = new btBoxShape(btVector3(BLDG_BLOCK_SIDE_LENGTH,
                                       BLDG_BLOCK_SIDE_LENGTH,
                                       BLDG_BLOCK_SIDE_LENGTH));
  ammoShape = new btSphereShape(btScalar(AMMO_RADIUS));
  collisionShapes.push_back(bldgShape);
  collisionShapes.push_back(ammoShape);
}

void Physics::update(int timeChange)
{
//  printf("Updating by: %d milliseconds.\n", timeChange);
  vector<WorldObject> changed;
  vector<PhysicsBody *> temp = physicsBodies;
  vector<WorldObject> newWorldObjects(temp.size(), WorldObject());
  if (timeChange)
    world->stepSimulation(btScalar(timeChange / 1000.0), 10);
  int i, result;
  for (i = 0; i < temp.size(); i++)
  {
		result = temp[i]->update();
    if (result == 1)
    {
      changed.push_back(temp[i]->getWorldObject());
    }
    if (result == 0)
    {
			//removeWorldObject(temp[i]->getWorldObject().getID());
      newWorldObjects[i] = temp[i]->getWorldObject();
		}
		else
		{
      newWorldObjects[i] = temp[i]->getWorldObject();
		}
  }
//  printf("changed.size() = %d, ", changed.size());
//  printf("newWorldObjects.size() = %d, ", newWorldObjects.size());
//  printf("worldObjects.size() = %d\n", worldObjects.size());
  worldObjects = newWorldObjects;
}

int Physics::isUniqueID(int id)
{
  int i;
  int result = 1;
  for (i = 0; result && i < worldObjects.size(); i++)
  {
    if (worldObjects[i].getID() == id)
    {
      result = 0;
    }
  }
  return result;
}

void Physics::addWorldObject(WorldObject worldObject)
{
  
  if (isUniqueID(worldObject.getID()))
  {
    PhysicsBody * newBody = new PhysicsBody(worldObject);
    // stall slow objects

    if (worldObject.getVelocity().mag() < .01)
    {
      newBody->setActivationState(ISLAND_SLEEPING);
    }
    world->addRigidBody(newBody->getRigidBody());
    physicsBodies.push_back(newBody);
//    worldObjects.push_back(worldObject);
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
  worldObjects.clear();
  physicsBodies.clear();
  int i = temp.size();
  // iterate backwards through the existing PhysicsBodies, removing them all.
  while (i-- > 0)
  {
    world->removeRigidBody(temp[i]);
  }
}

bool Physics::removeWorldObject(int id)
{
  bool removed = false;
  for (int i = 0; !removed && i < physicsBodies.size(); i++)
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

vector<WorldObject> Physics::getWorldObjects()
{
  return worldObjects;
}

vector<Vector> Physics::fileToBlockLocations(const char * fileName)
{
  vector<Vector> result;
  FILE * inFile = fopen(fileName, "r");
  int blockSize, numRead, x1, y1, z1, x2, y2, z2, xi, yi, zi;
  if ((numRead = fscanf(inFile, "blocksize: %d\n", &blockSize)) == 1)
  {
    while ((numRead = fscanf(inFile, "%d %d %d %d %d %d\n",&x1, &y1, &z1,
            &x2, &y2, &z2)) == 6)
      for (xi = x1; xi < x2; xi += blockSize)
        for (yi = y1; yi < y2; yi += blockSize)
          for (zi = z1; zi < z2; zi += blockSize)
            result.push_back(Vector((float)xi / blockSize * BLDG_BLOCK_SIDE_LENGTH,
                                    (float)yi / blockSize * BLDG_BLOCK_SIDE_LENGTH 
                                      + BLDG_BLOCK_SIDE_LENGTH / 2.0,
                                    (float)zi / blockSize * BLDG_BLOCK_SIDE_LENGTH));
    if (numRead != 0)
    {
//      printf("Unexpected data at end of file: ignored\n");
    }
  }
  else
  {
    printf("Unable to read block size.\n");
  }
  return result;
}

// The current implementation only adds blocks to the world, no reinitilization.
int Physics::loadFromFile(const char * fileName)
{
  Physics::emptyWorld();
  /////////////////////////////////////////////////////////////////////////////
  vector<Vector> toPlace = Physics::fileToBlockLocations(fileName);
  // This call will need to change with any movement to the placement of
  // the file load instruction.
  /////////////////////////////////////////////////////////////////////////////
  int i = 0;
#ifdef DEBUG
  printf("Loading file: \"%s\"\n", fileName);
#endif
  WorldObject *newObj;
  for (vector<Vector>::iterator it = toPlace.begin(); it < toPlace.end();
      it++)
  {
    Vector position = *it;

    newObj = new WorldObject(i++, 0, enumeration::DUMMY_BLOCK);
    newObj->setPosition(*it);
    addWorldObject(*newObj);
//    printf("adding object: %d\n", i);
  }
  return 1;
}
