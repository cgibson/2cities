#ifndef _PHYSOBJ_H_
#define _PHYSOBJ_H_

#include "Vector.h"

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
        Vector position;
        Vector velocity;
        Vector rotation;
        double rotationMag;
        float mass;

    public:
        PhysObj();
        PhysObj(Vector newPos, Vector newVel, Vector newRot, double newRotMag, float newMass);

	// Get Methods
        Vector getPosition()    { return position; }
        Vector getVelocity()    { return velocity; }
        Vector getRotation()    { return rotation; }
        double getRotationMag() { return rotationMag; }
        float  getMass()        { return mass; }

        // Set Methods
        void setPosition(Vector newPos)      { position = newPos; }
        void setVelocity(Vector newVel)      { velocity = newVel; }
        void setRotation(Vector newRot)      { rotation = newRot; }
        void setRotationMag(double newRotMag){ rotationMag = newRotMag; }
        void setMass(float newMass)          { mass = newMass; }

        //just to force abstraction for now. Delete this later.
        virtual void doSomething(){};
};


/**
 * AmmoUnit class
 */
class AmmoUnit: public PhysObj
{
    protected:
        float size;

    public:
        // Constructs off of PhysObj
        AmmoUnit();

        float getSize() { return size; }
        void  setSize(float newSize) { size = newSize; }

        virtual void doSomething() {};
};

/**
 * DummyAmmoUnit class
 */
class DummyAmmoUnit: public AmmoUnit
{
    public:
        DummyAmmoUnit() : AmmoUnit() {};

        void doSomething() {};
};

/**
 * BuildingUnit class
 */
class BuildingUnit: public PhysObj
{
    protected:
        float size;

    public:
        // Constructs off of PhysObj
        BuildingUnit();
        BuildingUnit(Vector newPos, Vector newRot, double newRotMag, float newMass, float newSize);

        float getSize() { return size; }
        void  setSize(float newSize) { size = newSize; }

        virtual void doSomething() {};
};

/**
 * DummyBuildingUnit class
 */
class DummyBuildingUnit: public BuildingUnit
{
    public:
        DummyBuildingUnit() : BuildingUnit() {};
        DummyBuildingUnit(Vector newPos, Vector newRot, double newRotMag, float newMass, float newSize) 
           : BuildingUnit(newPos, newRot, newRotMag, newMass, newSize) {};

        void doSomething() {};
};


#endif
