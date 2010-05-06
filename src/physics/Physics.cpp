#include "Physics.h"

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
  vector<WorldObject> changed;//////////////////////////////////////////////////////////
  if (timeChange)
    world->stepSimulation(btScalar(timeChange / 1000.0), 10);
  int i, j;
  bool found;
  for (i = 0; i < worldObjects.size(); i++)
  {
    found = false;
    for (j = 0; !found && j < physicsBodies.size(); j++)
    {
      if (physicsBodies[j].getID() == worldObjects[i].getID())
      {
        found = true;
        if (physicsBodies[j].update(&worldObjects[i]))
        {
          changed.push_back(worldObjects[i]);
        }
      }
    }
  }
  
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

void Physics::addWorldObject(WorldObject newObject)
{
  
  if (isUniqueID(newObject.getID()))
  {
    PhysicsBody * newBody = new PhysicsBody(newObject);
    // stall slow objects
    if (newObject.getPosition().mag() < .01)
    {
      newBody->setActivationState(ISLAND_SLEEPING);
    }
    world->addRigidBody(newBody);
    worldObjects.push_back(newObject);
    physicsBodies.push_back(*newBody);
  }
  else
  {
    // TODO provide a reaction to a non-unique ID value.
  }
}

vector<WorldObject> Physics::getWorldObjects()
{
  return worldObjects;
}

void Physics::addAmmo(AmmoUnit ammo)
{
  world->addRigidBody(ammo.getRigidBody());
//  ammo.getRigidBody()->setActivationState(ISLAND_SLEEPING);
  ammunition.push_back(ammo);
}

void Physics::addBuildingBlock(BuildingUnit bldg)
{
//  bldg.getRigidBody()->setActivationState(ISLAND_SLEEPING);
  world->addRigidBody(bldg.getRigidBody());
//  bldg.getRigidBody()->setActivationState(ISLAND_SLEEPING);
  buildingBlocks.push_back(bldg);
}

vector<AmmoUnit> Physics::getAmmo()
{
  vector<AmmoUnit> result;
  result = ammunition;
  return result;
}

vector<BuildingUnit> Physics::getBuildingBlocks()
{
  vector<BuildingUnit> result;
  result = buildingBlocks;
  return result;
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
  //Physics::emptyWorld();
  /////////////////////////////////////////////////////////////////////////////
  vector<Vector> toPlace = Physics::fileToBlockLocations(fileName);
  // This call will need to change with any movement to the placement of
  // the file load instruction.
  /////////////////////////////////////////////////////////////////////////////
  for (vector<Vector>::iterator it = toPlace.begin(); it < toPlace.end();
      it++)
  {
    Vector position = *it;
    addBuildingBlock(*(new DummyBuildingUnit(*it, Vector(0,0,0), Vector(0,0,0), 0)));
  }
  return 1;
}
