#include "PhysicsBody.h"
#include "../system/global.h"
#include <time.h>
//extern global::factory;

btScalar PhysicsBody::getMass(ObjectType type)
{
  //return btScalar(1.0);
  return btScalar(global::factory->getBlueprint(type).getMass());
}

PhysicsBody::PhysicsBody(WorldObject input):
/* btRigidBody(
    getMass(input.getType()),
    new btDefaultMotionState(), getShape(input.getType()), btVector3(0,0,0))*/
/*btRigidBody::btRigidBodyConstructionInfo(getMass(input.getType(), new btDefaultMotionState(btTransform()),
            getShape(input.getType()), btVector3(0,0,0))*/
btRigidBody(getCI(input))
{
  id = input.getID();
  playerID = input.getPlayerID();
  btTransform trans;
  Vector pos = input.getPosition();
  Vector vel = input.getVelocity();
  Quaternion ori = input.getOrientation();
  
  // set position
  trans.setIdentity();
  trans.setOrigin(VtobtV3(pos));
  btQuaternion quat;
  quat.setRotation(VtobtV3(ori.getAxis()), btScalar(ori.getAngle()));
  trans.setRotation(quat);
  setCenterOfMassTransform(trans);
  // set velocity
  setLinearVelocity(VtobtV3(vel));
    body = (btRigidBody *)(this);
}

btRigidBody::btRigidBodyConstructionInfo PhysicsBody::getCI(WorldObject input)
{
  btScalar mass = PhysicsBody::getMass(input.getType());
  btVector3 inertia(0,0,0);
  btCollisionShape * thisShape = getShape(input.getType());
  Quaternion quat = input.getOrientation();
  btQuaternion q2;
  q2.setRotation(VtobtV3(quat.getAxis()), btScalar(quat.getAngle()));
  btDefaultMotionState * ms = new btDefaultMotionState();
  
  if (mass != 0)
  {
    thisShape->calculateLocalInertia(mass, inertia);
  }
  btRigidBody::btRigidBodyConstructionInfo ci(mass, ms, thisShape, inertia);
  return ci;
}

bool PhysicsBody::update(WorldObject * wo)
{
  float epsilon = .1;
  Vector oldPos = wo->getPosition();
  Vector newPos = btV3toV(body->getCenterOfMassPosition());
  wo->setPosition(newPos);
  wo->setVelocity(btV3toV(body->getLinearVelocity()));
  Quaternion quat;
  btQuaternion btq = body->getOrientation();
  quat.setAxis(btV3toV(btq.getAxis()));
  quat.setAngle(btq.getAngle() * 180 / 3.14159 );
  wo->setOrientation(quat);
  wo->setForce(Vector(0,0,0));
  wo->setTimeStamp(time(NULL));
  return (newPos - oldPos).mag() > epsilon;
}

btCollisionShape *  PhysicsBody::getShape(ObjectType objectType)
{
  btCollisionShape * s;
  switch (objectType)
  {
/*    case SMALL_CUBE:
      return small_cube;
    case LARGE_SPHERE:
      return large_sphere;
    case MEDIUM_SPHERE:
      return medium_sphere;
    case SMALL_SPHERE: default:
      return small_sphere;*/
    case DUMMY_BLOCK:
      s = small_cube;break;
    case DUMMY_SPHERE:
      s = small_sphere;break;
  }
  //  btVector3
  return s;
}

/*
btCollisionShape * PhysicsBody::dummy_block = new btBoxShape(VtobtV3(
Vector(.5,.5,.5)));
//    global::factory->getBlueprint(DUMMY_BLOCK).getSize()));

btCollisionShape * PhysicsBody::dummy_sphere = new btSphereShape(
    btScalar(
//      global::factory->getBlueprint(DUMMY_SPHERE).getSize().mag()));
1));
*/

btCollisionShape * PhysicsBody::small_cube = new btBoxShape(VtobtV3(
Vector(.5, .5, .5)));

//    global::factory->getBlueprint(DUMMY_BLOCK).getSize()));
/*
btCollisionShape * PhysicsBody::large_sphere = new btSphereShape(
    btScalar(factory.getBlueprint(???).getSize().mag()));

btCollisionShape * PhysicsBody::medium_sphere = new btSphereShape(
    btScalar(factory.getBlueprint(???).getSize().mag()));
*/ 
btCollisionShape * PhysicsBody::small_sphere = new btSphereShape(
    btScalar(
1));
//global::factory->getBlueprint(DUMMY_SPHERE).getSize().mag()));

