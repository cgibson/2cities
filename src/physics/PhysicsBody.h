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
    btRigidBody::btRigidBodyConstructionInfo getCI(WorldObject);

    btRigidBody * body; 
    WorldObject * wo;
    ObjectType type;
    float forceScale;
    float boundForce(float);
    // Location to be drawn to, strength, far depth, event horizon depth
    bool drawnByBlackHoleAt(Vector, float, float, float);
    void setPosition(Vector newPos);
    void setVelocity(Vector newVel);
  public:
    static btScalar getMass(ObjectType type);
    static Vector btV3toV(btVector3 in) 
    {
      return Vector(in.getX(), in.getY(), in.getZ());
    }

    static btVector3 VtobtV3(Vector in)
    {
      return btVector3(btScalar(in.x()), btScalar(in.y()), btScalar(in.z()));
    }
    btRigidBody * getRigidBody() {return body;}
    PhysicsBody(WorldObject *);
//    PhysicsBody(WorldObject);
    int getID() {return wo->getID();}
    int getPlayerID() {return wo->getPlayerID();}
    int getType() {return wo->getType();}
    WorldObject * getWorldObject() {return wo;}
    int update();
    Vector getForce();
};

#endif
