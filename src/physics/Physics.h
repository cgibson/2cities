#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "btBulletDynamicsCommon.h"
#include "../state/Vector.h"
#include "../state/PhysObj.h"
#include <vector>

//#define AMMO_MASS 1
//#define BLDG_BLOCK_MASS 1

class Physics
{
  private:
    btDiscreteDynamicsWorld * world;
    btSequentialImpulseConstraintSolver * solver;
    btBroadphaseInterface * broadphase;
    btCollisionDispatcher * dispatcher;
    btDefaultCollisionConfiguration * collConf;
    btAlignedObjectArray<btCollisionShape *> collisionShapes;
    btRigidBody * groundBody;
    std::vector<AmmoUnit> ammunition;
    std::vector<BuildingUnit> buildingBlocks;
//    std::vector<btRigidBody *> bldgBodies;
//    std::vector<btRigidBody *> ammoBodies;
//    std::vector<Vector> oldAmmoPositions;
//    std::vector<Vector> oldBldgPositions;

//    btRigidBody * placePhysicsObject(btCollisionShape * shape, Vector location,
//            Vector velocity, btScalar mass);
//    btVector3 VectorTObtVector3(Vector in);
//    Vector VectorFrombtVector3(btVector3 in);
    std::vector<Vector> fileToBlockLocations(char * fileName);
//    void emptyWorld();
  public:
    btBoxShape * bldgShape;
    btSphereShape * ammoShape;
    
    std::vector<AmmoUnit> getAmmo();
    std::vector<BuildingUnit> getBuildingBlocks();
    void addAmmo(AmmoUnit ammo);
    void addBuildingBlock(BuildingUnit bldg);
    void exitPhysics();
    void initPhysics();
    // Advance the simulation by timeChange milliseconds
    void update(int timeChange);
//    void placeBlock(Vector location);
//    void placeAmmo(Vector location, Vector velocity);
    int loadFromFile(char * fileName);
};

#endif
