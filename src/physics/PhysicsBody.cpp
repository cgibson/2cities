#include "PhysicsBody.h"
#include "../system/global.h"
#include <time.h>

btScalar PhysicsBody::getMass(ObjectType type)
{
  return btScalar(global::factory->getBlueprint(type).getMass());
}

PhysicsBody::PhysicsBody(WorldObject worldObject):
btRigidBody(getCI(worldObject))
{
  wo = worldObject;
  btTransform trans;
  Vector pos = wo.getPosition();
  Vector vel = wo.getVelocity();
  Quaternion ori = wo.getOrientation();
  
  // set position
  trans.setIdentity();
  trans.setOrigin(VtobtV3(pos));
  btQuaternion quat;
  if (ori.getAngle() < -0.1 || ori.getAngle() > 0.1)
    quat.setRotation(VtobtV3(ori.getAxis()), btScalar(ori.getAngle()));
  trans.setRotation(quat);
  setCenterOfMassTransform(trans);
  // set velocity
  setLinearVelocity(VtobtV3(vel));
  body = (btRigidBody *)(this);
}

btRigidBody::btRigidBodyConstructionInfo PhysicsBody::getCI(WorldObject worldObject)
{
  btScalar mass = PhysicsBody::getMass(worldObject.getType());
  btVector3 inertia(0,0,0);
  btCollisionShape * thisShape = NULL;
#ifdef SERVER
  thisShape = global::factory->getShape(worldObject.getType());
#endif

  btDefaultMotionState * ms = new btDefaultMotionState();
  
  if (mass != 0)
  {
    thisShape->calculateLocalInertia(mass, inertia);
  }
  btRigidBody::btRigidBodyConstructionInfo ci(mass, ms, thisShape, inertia);
  return ci;
}

bool PhysicsBody::update()
{
  float epsilon = .1;
  Vector oldPos = wo.getPosition();
  Vector newPos = btV3toV(body->getCenterOfMassPosition());
  wo.setPosition(newPos);
  wo.setVelocity(btV3toV(body->getLinearVelocity()));
  Quaternion quat;
  btQuaternion btq = body->getOrientation();
  quat.setAxis(btV3toV(btq.getAxis()));
  quat.setAngle(btq.getAngle() * 180 / 3.14159 );
  wo.setOrientation(quat);
  wo.setForce(Vector(0,0,0));
  wo.setTimeStamp(time(NULL));
//  printf("newPos.mag() = %f\n", newPos.mag());
  return (newPos - oldPos).mag() > epsilon;
}

btCollisionShape * PhysicsBody::small_cube = new btBoxShape(VtobtV3(
Vector(.5, .5, .5)));

btCollisionShape * PhysicsBody::small_sphere = new btSphereShape(
    btScalar(
1));

