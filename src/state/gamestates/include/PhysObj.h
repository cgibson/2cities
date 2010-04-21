#ifndef _PHYSOBJ_H_
#define _PHYSOBJ_H_

#include "Vector.h"

class PhysObj
{
    protected:
        Vector position;
        Vector velocity;
        Vector direction;
        float mass;

    public:
        Vector getPosition(){return position;}
        Vector getVelocity(){return velocity;}
        Vector getDirection(){return direction;}
        float getMass(){return mass;}

        void setPosition(Vector newPos){position = newPos;}
        void setVelocity(Vector newVel){velocity = newVel;}
        void setDirection(Vector newDir){direction = newDir;}
        void setMass(float newMass){mass = newMass;}

        //just to force abstraction for now. Delete this later.
        virtual void doSomething();
};

#endif
