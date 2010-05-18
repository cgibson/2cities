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
  btQuaternion quat = trans.getRotation();
  if (ori.getAngle() < -0.001 || ori.getAngle() > 0.001)
    quat.setRotation(VtobtV3(ori.getAxis()), btScalar(ori.getAngle()));
  trans.setRotation(quat);
  setCenterOfMassTransform(trans);
  // set velocity
  setLinearVelocity(VtobtV3(vel));
//  btScalar linearDamping = getLinearDamping();
//  btScalar angularDamping = getAngularDamping();
//  printf("ld = %f, ad = %f\n", linearDamping, angularDamping);
//  setDamping(linearDamping * 100, angularDamping * 100);
  body = (btRigidBody *)(this);
}

btRigidBody::btRigidBodyConstructionInfo PhysicsBody::getCI(WorldObject worldObject)
{
  btScalar mass = PhysicsBody::getMass(worldObject.getType());
  btVector3 inertia(0,0,0);
  btCollisionShape * thisShape = NULL;
  thisShape = global::factory->getShape(worldObject.getType());

  btDefaultMotionState * ms = new btDefaultMotionState();
  
  if (mass != 0)
  {
    thisShape->calculateLocalInertia(mass, inertia);
  }
  btRigidBody::btRigidBodyConstructionInfo ci(mass, ms, thisShape, inertia);
//  printf("Friction was: %f,\t", ci.m_friction);
  ci.m_friction = 0.75;
//  printf("Friction is now: %f", ci.m_friction); 
  return ci;
}

float PhysicsBody::boundForce(float in)
{
  float bound = 100;
  if (in < -bound)
    return -bound;
  if (in > bound)
    return bound;
  return in;
}

Vector PhysicsBody::getForce()
{
  // everything has a default energy of 10
  float linear = boundForce(btV3toV(body->getInterpolationLinearVelocity()).mag());
  float angular = boundForce(btV3toV(body->getAngularVelocity()).mag());
  float force = boundForce(75);
//  printf("linear = %f\n", linear);
//  printf("angular = %f\n", angular);
//  printf("force = %f\n", force);
  return Vector(linear, angular, force);
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
//  btTransform trans = body->getCenterOfMassTransform();
//  btQuaternion btq = trans.getRotation();
  quat.setAxis(btV3toV(btq.getAxis()));
  quat.setAngle(btq.getAngle() * 180 / 3.14159 );
  wo.setOrientation(quat);
  Vector force = getForce();
  wo.setForce(force);
  wo.setTimeStamp(global::elapsed_ms());
  return (newPos - oldPos).mag() > epsilon;
}

/*
void PhysicsBody::applyCentralForce(const btVector3& force)
{
  setm_totalForce += force*getLinearFactor();
  printf("updating object %d with force of magnitude %f.\n", wo->getID(), btV3toV(force).mag());
}

void PhysicsBody::applyGravity()
{
  printf("PhysicsBody::applyGravity() called.\n");
  if (isStaticOrKinematicObject())
    return;
  applyCentralForce(m_gravity);
}*/

btCollisionShape * PhysicsBody::small_cube = new btBoxShape(VtobtV3(
Vector(.5, .5, .5)));

btCollisionShape * PhysicsBody::small_sphere = new btSphereShape(
    btScalar(
1));

