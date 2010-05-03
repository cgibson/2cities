#ifndef _WORLDOBJECT_H_
#define _WORLDOBJECT_H_

#include "../helper/Vector.h"
#include "../system/enum.h"

/**
 * Incomplete WorldObject class.
 *
 * @author Kennedy Owen
 * @version 2 May 2010
 */


/**
 * WorldObject class. Note no acceleration/gravity Vector; applying forces
 * is handled by the Bullet physics classes.
 */
class WorldObject
{
    protected:
        unsigned int id;
        unsigned int playerid;
        Vector position;
        Vector velocity;
        Vector force;
        //Quaternion orientation;
        enumeration::E_SHAPE shape; //SMALL_SPHERE, MEDIUM_SPHERE, LARGE_SPHERE, SMALL_CUBE, etc
        enumeration::E_PHYSICS physics; //NONE, DYNAMIC, STATIC
        long timestamp; //change to int?

        virtual void draw();
        virtual void update(int elapsedTime);
        virtual void think(int elapsedTime);

    public:
        WorldObject(unsigned int newid, unsigned int newplayerid, enumeration::E_SHAPE newshape)
        {
            id = newid;
            playerid = newplayerid;
            shape = newshape;
        }
        unsigned int getID() {return id;}
        unsigned int getPlayerID() {return playerid;}
        Vector getPosition() {return position;}
        Vector getVelocity() {return velocity;}
        Vector getForce() {return force;}
        //Quaternion getOrientation() {return orientation;}
        enumeration::E_SHAPE getShape() {return shape;}
        enumeration::E_PHYSICS getPhysics() {return physics;}
        void setPosition(Vector newposition) {position = newposition;}
        void setVelocity(Vector newvelocity) {velocity = newvelocity;}
        void setForce(Vector newforce) {force = newforce;}
        //void setOrientation(enumeration::Quaternion neworientation) {orientation = neworientation;}
        void setPhysics(enumeration::E_PHYSICS newphysics) {physics = newphysics;}
};

#endif
