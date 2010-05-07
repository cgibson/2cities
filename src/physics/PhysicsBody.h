#ifndef _PHYSICSBODY_H_
#define _PHYSICSBODY_H_

//#include "../system/global.h"
#include "../system/enum.h"
#include "../scene/WorldObject.h"
#include "../helper/Vector.h"
#include "btBulletDynamicsCommon.h"
#include "../math/quaternion.h"
//using namespace enumeration;

class PhysicsBody : public btRigidBody
{
  private:
    btRigidBody::btRigidBodyConstructionInfo getCI(WorldObject input);

    btRigidBody * body; 
    static btCollisionShape * small_sphere;
/*    btCollisionShape * medium_sphere;
    btCollisionShape * large_sphere;
*/    static btCollisionShape * small_cube;

    static btCollisionShape * dummy_block;
    static btCollisionShape * dummy_sphere;
    ObjectType type;
    int id;
    int playerID;
    
  public:
    static btScalar getMass(ObjectType type);
    static btCollisionShape * getShape(ObjectType type);
    static Vector btV3toV(btVector3 in) 
    {
      return Vector(in.getX(), in.getY(), in.getZ());
    }

    static btVector3 VtobtV3(Vector in)
    {
      return btVector3(btScalar(in.x()), btScalar(in.y()), btScalar(in.z()));
    }
    btRigidBody * getRigidBody() {return body;}
    PhysicsBody(WorldObject);    
    int getID() {return id;}
    int getPlayerID() {return playerID;}
    int getType() {return type;}
    bool update(WorldObject * wo);
};

#endif
