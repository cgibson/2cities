#ifndef _WORLDOBJECT_H_
#define _WORLDOBJECT_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "../helper/Vector.h"
#include "../system/enum.h"
#include "../math/quaternion.h"
#include "../scene/factory.h"

/**
 * Incomplete WorldObject class.
 *
 * @author Kennedy Owen
 * @version 2 May 2010
 */

using namespace enumeration;

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
        Quaternion orientation;
        ObjectType type; //SMALL_SPHERE, MEDIUM_SPHERE, LARGE_SPHERE, SMALL_CUBE, etc
        PhysicsState physics; //NONE, DYNAMIC, STATIC
        long timestamp; //change to int?

    public:
        WorldObject(unsigned int newid, unsigned int newplayerid, ObjectType newtype)
        {
            id = newid;
            playerid = newplayerid;
            type = newtype;
        }
        
        virtual void draw();
        virtual void update(int elapsedTime);
        virtual void think(int elapsedTime);

        unsigned int getID() {return id;}
        unsigned int getPlayerID() {return playerid;}
        Vector getPosition() {return position;}
        Vector getVelocity() {return velocity;}
        Vector getForce() {return force;}
        Quaternion getOrientation() {return orientation;}
        ObjectType getType() {return type;}
        PhysicsState getPhysics() {return physics;}
        void setPosition(Vector newposition) {position = newposition;}
        void setVelocity(Vector newvelocity) {velocity = newvelocity;}
        void setForce(Vector newforce) {force = newforce;}
        void setOrientation(Quaternion neworientation) {orientation = neworientation;}
        void setPhysics(PhysicsState newphysics) {physics = newphysics;}
};

#endif
