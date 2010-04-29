#include "Physics.h"
#include <stdio.h>

#define GROUND_HEIGHT 10
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
  btAlignedObjectArray<btCollisionShape *> collisionShapes;
  collisionShapes.push_back(ground);

  btTransform groundTrans;
  groundTrans.setIdentity();
  groundTrans.setOrigin(btVector3(0,-GROUND_HEIGHT / 2.0,0));
  btRigidBody::btRigidBodyConstructionInfo grbInfo(btScalar(0.),
    new btDefaultMotionState(groundTrans), ground, btVector3(0,0,0));
  // Put the ground into the world
  groundBody = new btRigidBody(grbInfo);
  world->addRigidBody(groundBody);
  bldgShape = new btBoxShape(btVector3(BLDG_BLOCK_SIDE_LENGTH,
                                       BLDG_BLOCK_SIDE_LENGTH,
                                       BLDG_BLOCK_SIDE_LENGTH));
  ammoShape = new btSphereShape(btScalar(AMMO_RADIUS));
  collisionShapes.push_back(bldgShape);
  collisionShapes.push_back(ammoShape);
}

void Physics::update(int timeChange)
{
  if (timeChange)
    world->stepSimulation(btScalar(timeChange / 1000.0), 10);
}

void Physics::addAmmo(AmmoUnit ammo)
{
  world->addRigidBody(ammo.getRigidBody());
  ammunition.push_back(ammo);
}

void Physics::addBuildingBlock(BuildingUnit bldg)
{
  world->addRigidBody(bldg.getRigidBody());
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



vector<Vector> Physics::fileToBlockLocations(char * fileName)
{
  vector<Vector> result;
  FILE * inFile = fopen(fileName, "r");
  int blockSize, numRead, x1, y1, z1, x2, y2, z2, xi, yi, zi;
  if ((numRead = fscanf(inFile, "Block Size: %d\n", &blockSize)) == 1)
  {
    while ((numRead = fscanf(inFile, "%d %d %d %d %d %d\n",&x1, &y1, &z1,
            &x2, &y2, &z2)) == 6)
      for (xi = x1; xi < x2; xi += blockSize)
        for (yi = y1; yi < y2; yi += blockSize)
          for (zi = z1; yi < y2; yi += blockSize)
            result.push_back(Vector(xi / blockSize * BLDG_BLOCK_SIDE_LENGTH,
                                    yi / blockSize * BLDG_BLOCK_SIDE_LENGTH 
                                      + BLDG_BLOCK_SIDE_LENGTH / 2.0,
                                    zi / blockSize * BLDG_BLOCK_SIDE_LENGTH));
    if (numRead != 0)
    {
      printf("Unexpected data at end of file: ignored\n");
    }
  }
  else
  {
    printf("Unable to read block size.\n");
  }
  return result;
}

// The current implementation only adds blocks to the world, no reinitilization.
int Physics::loadFromFile(char * fileName)
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
