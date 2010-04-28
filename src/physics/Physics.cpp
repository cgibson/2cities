#include "Physics.h"
#include <stdio.h>
using namespace std;

void Physics::emptyWorld()
{
  vector<btRigidBody *>::iterator it;
  oldBldgPositions.clear();
  for (it = bldgBodies.begin();
      it < bldgBodies.end(); it++)
  {
    delete *it;
    bldgBodies.erase(it);
  }
  oldAmmoPositions.clear();
  for (it = ammoBodies.begin();
      it < ammoBodies.end(); it++)
  {
    delete *it;
    ammoBodies.erase(it);
  }
}

void Physics::exitPhysics()
{
  Physics::emptyWorld();
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
          btScalar(10.), btScalar(100.)));
  btAlignedObjectArray<btCollisionShape *> collisionShapes;
  collisionShapes.push_back(ground);

  btTransform groundTrans;
  groundTrans.setIdentity();
  groundTrans.setOrigin(btVector3(0,-56,0));
  // groundShape->calculateLocalInertia(btScalar mass, btVector3 localInertia): would set ground into motion
  
  btRigidBody::btRigidBodyConstructionInfo grbInfo(btScalar(0.), new btDefaultMotionState(groundTrans), ground, btVector3(0,0,0));
  // Put the ground into the world
  groundBody = new btRigidBody(grbInfo);
//    new btRigidBody::btRigidBodyConstructionInfo(btScalar(0.),
//      new btDefaultMotionState(groundTrans), ground, btVector3(0,0,0)));
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
  int i;
  btTransform trans;
  vector<Vector> tempAmmo;
  for (i = 0; i < ammoBodies.size(); i++)
  {
    ammoBodies[i]->getMotionState()->getWorldTransform(trans);
    tempAmmo.push_back(VectorFrombtVector3(trans.getOrigin()));
  }
  vector<Vector> tempBldg;
  for (i = 0; i < bldgBodies.size(); i++)
  {
    bldgBodies[i]->getMotionState()->getWorldTransform(trans);
    tempBldg.push_back(VectorFrombtVector3(trans.getOrigin()));
  }
  oldAmmoPositions = tempAmmo;
  oldBldgPositions = tempBldg;
}

btVector3 Physics::VectorTObtVector3(Vector in)
{
  return btVector3(btScalar(in.x()), btScalar(in.y()), btScalar(in.z()));
}

Vector Physics::VectorFrombtVector3(btVector3 in)
{
  return Vector(in.getX(), in.getY(), in.getZ());
}

btRigidBody * Physics::placePhysicsObject(btCollisionShape * shape, Vector location,
        Vector velocity, btScalar mass)
{
  btTransform trans;
  trans.setIdentity();
  btVector3 loc = Physics::VectorTObtVector3(location);
  btVector3 vel = Physics::VectorTObtVector3(velocity);
  shape->calculateLocalInertia(mass, vel);
  trans.setOrigin(loc);
  btRigidBody * result = new btRigidBody(
    btRigidBody::btRigidBodyConstructionInfo(mass,
      new btDefaultMotionState(trans), shape, vel));
  world->addRigidBody(result);
  Physics::update(0); // reset the old position vectors
  return result;
}


void Physics::placeBlock(Vector location)
{
  Physics::placePhysicsObject(bldgShape, location, Vector(0, 0, 0),
    BLDG_BLOCK_MASS);
}

void Physics::placeAmmo(Vector location, Vector velocity)
{
  Physics::placePhysicsObject(ammoShape, location, velocity, AMMO_MASS);
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
            result.push_back(Vector(xi / BLDG_BLOCK_SIDE_LENGTH,
                                    yi / BLDG_BLOCK_SIDE_LENGTH,
                                    zi / BLDG_BLOCK_SIDE_LENGTH));
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

int Physics::loadFromFile(char * fileName)
{
  Physics::emptyWorld();
  /////////////////////////////////////////////////////////////////////////////
  vector<Vector> toPlace = Physics::fileToBlockLocations(fileName);
  // This call will need to change with any movement to the placement of
  // the file load instruction.
  /////////////////////////////////////////////////////////////////////////////
  for (vector<Vector>::iterator it = toPlace.begin(); it < toPlace.end();
      it++)
  {
    placeBlock(*it);
  }
  return 1;
}

vector<AmmoUnit> Physics::getAmmo()
{
  vector<AmmoUnit> result;
  int i;
  for (i = 0; i < ammoBodies.size(); i++)
  {
    AmmoUnit current;
    btTransform trans;
    ammoBodies[i]->getMotionState()->getWorldTransform(trans);
    current.setPosition(VectorFrombtVector3(trans.getOrigin()));
    Vector vel = Vector(0,0,0); // default velocity
    if (i < oldAmmoPositions.size())
      vel = VectorFrombtVector3(trans.getOrigin()) - oldAmmoPositions[i];
    current.setVelocity(vel);
    //current.setDirection(VectorFrombtVector3(trans.getRotation()));
    //The rotation is supplied in the form of a quaternion, the project
    //currently lacks a 4 dimensional Vector class so for the moment the
    //default value of (0,1,0) is being supplied. This must be replaced
    //with some system for communicating the rotation properly.
    //I'm suggesting a rotation vector and distance, due to the simplicity
    //of putting it into OpenGL, as well as removing it from the quaternion
    //supplied here.
    current.setDirection(Vector(0,1,0));// TODO replace this, may require a
                                        //      refactorization of the
                                        //      PhysObj class.
    current.setMass(AMMO_MASS);
    result.push_back(current);
  }
  return result;
}

vector<BuildingUnit> Physics::getBuildingBlocks()
{
  vector<BuildingUnit> result;
  int i;

  for (i = 0; i < bldgBodies.size(); i++)
  {
    BuildingUnit current;
    btTransform trans;
    bldgBodies[i]->getMotionState()->getWorldTransform(trans);
    current.setPosition(VectorFrombtVector3(trans.getOrigin()));
    Vector vel = Vector(0,0,0); // default velocity
    if (i < oldBldgPositions.size())
      vel = VectorFrombtVector3(trans.getOrigin()) - oldAmmoPositions[i];
    current.setVelocity(vel);
    //current.setDirection(VectorFrombtVector3(trans.getRotation()));
    //The rotation is supplied in the form of a quaternion, the project
    //currently lacks a 4 dimensional Vector class so for the moment the
    //default value of (0,1,0) is being supplied. This must be replaced
    //with some system for communicating the rotation properly.
    //I'm suggesting a rotation vector and distance, due to the simplicity
    //of putting it into OpenGL, as well as removing it from the quaternion
    //supplied here.
    current.setDirection(Vector(0,1,0));// TODO replace this, may require a
                                        //      refactorization of the
                                        //      PhysObj class.
    current.setMass(BLDG_BLOCK_MASS);
    result.push_back(current);
  }
  return result;
}
