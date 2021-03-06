#include "PhysicsBody.h"
#include "../system/global.h"

btScalar PhysicsBody::getMass(ObjectType type)
{
  return btScalar(global::factory->getBlueprint(type).getMass());
}

void PhysicsBody::setPosition(Vector in)
{
  if (body != NULL)
  {
	  btTransform trans = body->getCenterOfMassTransform();
	  trans.setOrigin(VtobtV3(in));
	  body->setCenterOfMassTransform(trans);
	}
}

void PhysicsBody::setVelocity(Vector in)
{
  if (body != NULL)
  	body->setLinearVelocity(VtobtV3(in));
}

PhysicsBody::PhysicsBody(WorldObject *worldObject):
btRigidBody(getCI(*worldObject))
{
  wo = worldObject;
  btTransform trans;
  Vector pos = wo->getPosition();
  Vector vel = wo->getVelocity();
  Quaternion ori = wo->getOrientation();
  
  trans.setIdentity();
  trans.setOrigin(VtobtV3(pos));
  btQuaternion quat = trans.getRotation();
  if (ori.getAngle() < -0.001 || ori.getAngle() > 0.001)
    quat.setRotation(VtobtV3(ori.getAxis()), btScalar(ori.getAngle()));
  trans.setRotation(quat);
  setCenterOfMassTransform(trans);
  setLinearVelocity(VtobtV3(vel));
  body = (btRigidBody *)(this);
  forceScale = 0.0f;
}

btRigidBody::btRigidBodyConstructionInfo PhysicsBody::getCI(WorldObject worldObject)
{
  ObjectType type = worldObject.getType();
  btScalar mass = PhysicsBody::getMass(type);
  btVector3 inertia(0,0,0);
  btCollisionShape * thisShape = NULL;
  thisShape = global::factory->getShape(type);

  btDefaultMotionState * ms = new btDefaultMotionState();
  
  if (mass != 0)
  {
    thisShape->calculateLocalInertia(mass, inertia);
  }
  btRigidBody::btRigidBodyConstructionInfo ci(mass, ms, thisShape, inertia);
  ci.m_friction = .75;
  ci.m_restitution = 0.01;
  ci.m_linearDamping = btScalar(.1);
  ci.m_angularDamping = btScalar(.1);
  if (type == CLUSTER_BOMB || type == BLACK_HOLE || type == AIR_STRIKE)
  {
    ci.m_friction = 9.99999;
    ci.m_linearDamping = btScalar(.25);
  }
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

void PhysicsBody::setToCull()
{
  setPosition(Vector(0,-1000, 0));
  setVelocity(Vector(0, -1000, 0));
}

bool PhysicsBody::drawnByBlackHoleAt(Vector loc, float strength,
                                     float far,float eh)
{
  Vector line = loc - wo->getPosition();
  if (line.mag() < far)
  {
		if (line.mag() < eh)
		{
      setToCull();
			return false;
		}
		else
		{
			float force = (far - (line.mag() - eh)) / (far - eh) * strength * getMass((enumeration::ObjectType)getType());
			line.norm();
			btVector3 forceApp = btVector3(line.x() * force,
			                               line.y() * force,
			                               line.z() * force);
			                               #ifdef DEBUG
                                     //printf("Force of %f applied to object by BlackHole at %s, type: %d\n", force, wo->getPosition().str(), (int)getType());
                                     #endif
//      body->setGravity(forceApp);
//			applyCentralForce(forceApp);
        activate(true);
        applyCentralImpulse(forceApp);
		}
  }
  return true;
}

int PhysicsBody::update()
{
  float epsilon = .1;
  Vector oldPos = wo->getPosition();
  Vector newPos = btV3toV(body->getCenterOfMassPosition());
  wo->setPosition(newPos);
  wo->setVelocity(btV3toV(body->getLinearVelocity()));
  Quaternion quat;
  btQuaternion btq = body->getOrientation();
//  btTransform trans = body->getCenterOfMassTransform();
//  btQuaternion btq = trans.getRotation();
  quat.setAxis(btV3toV(btq.getAxis()));
  quat.setAngle(btq.getAngle() * 180 / 3.14159 );
  wo->setOrientation(quat);
  Vector force = getForce();
  wo->setForce(force);
  wo->setTimeStamp(global::elapsed_ms());
//  if (wo->getID() >= 10000)
//  {
//    if (drawnByBlackHoleAt(Vector(0, 100, 0), 10, 1000, 10))
//      return -1;
//  }
//  if (wo->getID() < 10000 && wo.getID() % 2 && (body->getActivationState() & ISLAND_SLEEPING))
//    if (drawnByBlackHoleAt(Vector(-10, 10, 0), 100, 100, .5))
//      return -1;
//  if (wo.getID() < 10000 && wo.getID() % 2 - 1 && (body->getActivationState() & ISLAND_SLEEPING))
//		if (drawnByBlackHoleAt(Vector(10, 10, 0), 100, 100, .5))
//		  return -1;
  if ((newPos - oldPos).mag() > epsilon)
    return 1;
  return 0;
}
