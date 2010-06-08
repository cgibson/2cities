#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <stdio.h>
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../helper/Vector.h"
#include "../scene/WorldObject.h"
#include "PhysicsBody.h"

#define GROUND_HEIGHT 10

//#define AMMO_MASS 1
//#define AMMO_RADIUS 1
//#define BLDG_BLOCK_MASS 1
//#define BLDG_BLOCK_SIDE_LENGTH 1.

class Physics
{
  private:
    int nextBlockNumber;
    void InsertNewBlock(ObjectType type, Vector position);
    btDiscreteDynamicsWorld * world;
    btSequentialImpulseConstraintSolver * solver;
    btBroadphaseInterface * broadphase;
    btCollisionDispatcher * dispatcher;
    btDefaultCollisionConfiguration * collConf;
    btAlignedObjectArray<btCollisionShape *> collisionShapes;
    btRigidBody * groundBody;
    std::vector<WorldObject> recentChanges;
    std::vector<Vector> fileToBlockLocations(const char * fileName);
    int isUniqueID(int id);
    int isNotWorldObject(btCollisionObject * toTest);
    std::vector<btGhostObject *>ghosts;
  public:
    int getNextBlockNumber() {return nextBlockNumber;}
    int insertWorldObject(WorldObject * worldObject); // supplies a unique ID
    PhysicsBody * getPBfromCO(btCollisionObject * in);
    PhysicsBody * getPBfromID(int in);
    btDiscreteDynamicsWorld * getWorld();
    void addGhost(btGhostObject * ghost);
    int removeGhost(btGhostObject * ghost);
    std::vector<PhysicsBody *> physicsBodies;
//    static void tickCallback(btDynamicsWorld *dWorld,
//      btScalar timeChange);
    void addWorldObject(WorldObject *newObject); // does not supply a unique ID
    std::vector<WorldObject *> getWorldObjects();
    
    void exitPhysics();
    void initPhysics();
    // Advance the simulation by timeChange milliseconds
    void update(int timeChange);
    int loadFromFile(const char * fileName);
    bool removeWorldObject(int);
    void emptyWorld();
};

#endif
