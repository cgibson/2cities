#ifndef _PHYSOBJ_H_
#define _PHYSOBJ_H_

#include "../helper/Vector.h"
#include "btBulletDynamicsCommon.h"

#define AMMO_MASS 1
#define AMMO_RADIUS 1
#define BLDG_BLOCK_MASS 1
#define BLDG_BLOCK_SIDE_LENGTH 1.


/**
 * Contains PhysObj, AmmoUnit, DummyAmmoUnit, BuildingUnit,
 * DummyBuildingUnit classes.
 *
 * @author Kennedy Owen
 * @version 25 April 2010
 */

/**
 * PhysObj class
 */
class PhysObj
{
    protected:
        btRigidBody * rigidBody;
        Vector VectorFrombtVector3(const btVector3& in)
        {
          return Vector(in.getX(), in.getY(), in.getZ());
        }
        btVector3 btVector3FromVector(Vector in)
        {
          return btVector3(btScalar(in.x()), btScalar(in.y()), btScalar(in.z()));
        }
        
        btTransform trans()
        {
          btTransform transform;
          transform.setIdentity();
          if (rigidBody != NULL)
            transform = rigidBody->getCenterOfMassTransform();
          return transform;
        }
        
        void initialize(Vector newPos, Vector newVel, Vector newRot, double newRotMag, double mass, btCollisionShape * shape)
        {
          btTransform transform;
          transform.setIdentity();
          btVector3 inertia(0,0,0);
          if (mass != 0)
          {
            shape->calculateLocalInertia(btScalar(mass), inertia);
          }
          btDefaultMotionState * motionState = new btDefaultMotionState(transform);
          btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState,
            shape, inertia);
          rigidBody = new btRigidBody(rbInfo);
          setPosition(newPos);
          setVelocity(newVel);
          setRotation(newRot);
          setRotationMag(newRotMag);
        }
        
    public:
        //void setRigidbody(btRigidBody newRigidBody) {rigidBody = newRigidBody;}
        btRigidBody * getRigidBody() {return rigidBody;}

        PhysObj(Vector newPos, Vector newVel, Vector newRot, double newRotMag, double mass, btCollisionShape * shape)
        {
          initialize(newPos, newVel, newRot, newRotMag, mass, shape);
        }

        PhysObj(double mass, btCollisionShape * shape)
        {
          initialize(Vector(0,0,0), Vector(0,0,0), Vector(1,0,0), 0, mass, shape);
        }

        Vector getPosition()
        {
          return VectorFrombtVector3(trans().getOrigin());
        }

        Vector getVelocity()
        {
          return VectorFrombtVector3(rigidBody->getInterpolationLinearVelocity());
        }

        Vector getRotation()
        {
          return VectorFrombtVector3(trans().getRotation().getAxis());
        }
        
        double getRotationMag() {
          return trans().getRotation().getAngle();
        }
        
        void setPosition(Vector newPos)
        {
          btTransform transform = trans();
          transform.setOrigin(btVector3FromVector(newPos));
          
          rigidBody->setCenterOfMassTransform(transform);
        }
        
        void setVelocity(Vector newVel)
        {
          rigidBody->setLinearVelocity(btVector3FromVector(newVel));
        }
        
        void setRotation(Vector newRot)
        {
          if (newRot.x() == 0 && newRot.y() == 0 && newRot.z() == 0)
          {
            setRotationMag(0.);
            setRotation(Vector(0, 1, 0));
          }
          else
          {
            btTransform transform = trans();
            btQuaternion quat = transform.getRotation();
            quat.setRotation(btVector3FromVector(newRot), quat.getAngle());
            transform.setRotation(quat);
            rigidBody->setWorldTransform(transform);
          }
        }
        
        void setRotationMag(double newRotMag)
        {
          btTransform transform = trans();
          btQuaternion quat = transform.getRotation();
          quat.setRotation(quat.getAxis(), btScalar(newRotMag));
          transform.setRotation(quat);
          rigidBody->setWorldTransform(transform);
        }
        //just to force abstraction for now. Delete this later.
        virtual void doSomething() {};
};


/**
 * AmmoUnit class
 */
class AmmoUnit: public PhysObj
{
    public:
        AmmoUnit(double mass, btCollisionShape * shape):
          PhysObj(mass, shape) {}
        AmmoUnit(Vector pos, Vector vel, Vector rot, double rotMag, double mass,
                btCollisionShape * shape):
          PhysObj(pos, vel, rot, rotMag, mass, shape) {}
    
        virtual void doSomething() {};
};

/**
 * DummyAmmoUnit class
 */
class DummyAmmoUnit: public AmmoUnit
{
    protected:
        static btCollisionShape * shape;
        int init;
        btCollisionShape * getShape()
        {
/*          if (shape == null)
          {
            shape = new btSphereShape(btScalar(AMMO_RADIUS));
          }*/
          return shape;
        }
    public:
        DummyAmmoUnit(): //AmmoUnit(1., NULL) {}
          AmmoUnit(getMass(), getShape()) {}
        DummyAmmoUnit(Vector pos, Vector vel, Vector rot, double rotMag): //AmmoUnit(1, NULL) {}
          AmmoUnit(pos, vel, rot, rotMag, getMass(), getShape()) {}

        void doSomething() {};
        double getMass() { return AMMO_MASS; }
};

/**
 * BuildingUnit class
 */
class BuildingUnit: public PhysObj
{
    public:
        BuildingUnit(double mass, btCollisionShape * shape):
          PhysObj(mass, shape) {}
        BuildingUnit(Vector pos, Vector vel, Vector rot, double rotMag, double mass,
                btCollisionShape * shape):
          PhysObj(pos, vel, rot, rotMag, mass, shape) {}
        virtual void doSomething() {};
};

/**
 * DummyBuildingUnit class
 */
class DummyBuildingUnit: public BuildingUnit
{
    protected:
        static btCollisionShape * shape;
        btCollisionShape * getShape()
        {
/*          if (shape == null)
          {
            shape = new btBoxShape(btVector3(BLDG_BLOCK_SIDE_LENGTH / 2,
                                             BLDG_BLOCK_SIDE_LENGTH / 2,
                                             BLDG_BLOCK_SIDE_LENGTH / 2));
          }*/
          return shape;
        }
        
    public:
        DummyBuildingUnit():
          BuildingUnit(getMass(), getShape()) {}
        DummyBuildingUnit(Vector pos, Vector rot, float rotMag, float mass, float size):
          BuildingUnit(pos, Vector(0,0,0), rot, rotMag, getMass(), getShape()) {}
        DummyBuildingUnit(Vector pos, Vector vel, Vector rot, double rotMag):
          BuildingUnit(pos, vel, rot, rotMag, getMass(), getShape()) {}

        void doSomething() {};
        static double getMass() { return BLDG_BLOCK_MASS; }
};


#endif
