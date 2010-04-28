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

        Vector getPosition()    {return position;}
        Vector getVelocity()    {return velocity;}
        Vector getRotation()    {return rotation;}
        double getRotationMag() {return rotationMag;}
        float getMass()         {return mass;}

        void setPosition(Vector newPos){position = newPos;}
        void setVelocity(Vector newVel){velocity = newVel;}
        void setRotation(Vector newRot){rotation = newRot;}
        void setRotationMag(double newRotMag){rotationMag = newRotMag;}
        void setMass(float newMass){mass = newMass;}

        //just to force abstraction for now. Delete this later.
        virtual void doSomething(){};
};


/**
 * AmmoUnit class
 */
class AmmoUnit: public PhysObj
{
    //add any additional fields/functions here
};


/**
 * DummyAmmoUnit class
 */
class DummyAmmoUnit: public AmmoUnit
{
    //add any extra fields or functions here
    public:
        DummyAmmoUnit();
        void doSomething();
};


/**
 * BuildingUnit class
 */
class BuildingUnit: public PhysObj
{
    //add any extra fields or functions here
    protected:
        float size;

    public:
        BuildingUnit(){ size = 1; };
        virtual void doSomething(){};
};


/**
 * DummyBuildingUnit class
 */
class DummyBuildingUnit: public BuildingUnit
{
    public:
        DummyBuildingUnit();
        DummyBuildingUnit(Vector newPos, Vector newRot, double newRotMag, float newMass, float newSize);
        void doSomething(){};
};


#endif
