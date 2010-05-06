#include "PhysicsBody.h"
#include "../system/global.h"
//extern global::factory;

btScalar PhysicsBody::getMass(ObjectType type)
{
  return btScalar(global::factory->getBlueprint(type).getMass());
}

PhysicsBody::PhysicsBody(WorldObject input): btRigidBody(
    getMass(input.getType()),
    new btDefaultMotionState(), getShape(input.getType()), btVector3(0,0,0))
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
  setCenterOfMassTransform(trans);
  // set velocity
  setLinearVelocity(VtobtV3(vel));
  // set rotation
  btQuaternion quat;
  quat.setRotation(VtobtV3(ori.getAxis()), btScalar(ori.getAngle()));
}

bool PhysicsBody::update(WorldObject * wo)
{
  float epsilon = .1;
  Vector oldPos = wo->getPosition();
  Vector newPos = btV3toV(getCenterOfMassPosition());
  wo->setPosition(newPos);
  wo->setVelocity(btV3toV(getLinearVelocity()));
  Quaternion quat;
  btQuaternion btq = getOrientation();
  quat.setAxis(btV3toV(btq.getAxis()));
  quat.setAngle(btq.getAngle());
  wo->setOrientation(quat);
  //wo->setForce(Vector(0,0,0));
  // TODO set timestamp?
  return (newPos - oldPos).mag() > epsilon;
}

btCollisionShape *  PhysicsBody::getShape(ObjectType objectType)
{
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
      return small_cube;
    case DUMMY_SPHERE:
      return small_sphere;
  }
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
Vector(.5,.5,.5)));

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

