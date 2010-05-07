#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <stdio.h>
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "../helper/Vector.h"
#include "../scene/WorldObject.h"
#include "PhysicsBody.h"

#define GROUND_HEIGHT 10

//#define AMMO_MASS 10
//#define BLDG_BLOCK_MASS 1

#define AMMO_MASS 1
#define AMMO_RADIUS 1
#define BLDG_BLOCK_MASS 1
#define BLDG_BLOCK_SIDE_LENGTH 1.

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
    std::vector<WorldObject> worldObjects;
    std::vector<WorldObject> recentChanges;
    std::vector<PhysicsBody> physicsBodies;
    std::vector<Vector> fileToBlockLocations(const char * fileName);
    int isUniqueID(int id);
//    void emptyWorld();
  public:
    btBoxShape * bldgShape;
    btSphereShape * ammoShape;
    void addWorldObject(WorldObject newObject);
    std::vector<WorldObject> getWorldObjects();
    
    void exitPhysics();
    void initPhysics();
    // Advance the simulation by timeChange milliseconds
    void update(int timeChange);
    int loadFromFile(const char * fileName);
};

#endif
