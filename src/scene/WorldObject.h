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
#include <stdint.h>
#include <time.h>

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
        unsigned int bldgid;
        Vector position;
        Vector velocity;
        Vector force;
        Quaternion orientation;
        ObjectType type;
        PhysicsState physics; //NONE, DYNAMIC, STATIC
        uint64_t timestamp;

    public:
        // WO Derivative Specific Functions
        WorldObject();
        WorldObject(unsigned int newid, unsigned int newplayerid, ObjectType newtype);
        WorldObject(const WorldObject& newObj);
        WorldObject(unsigned char *bufPtr);
        
        // drops the object below the scene far enough to escape display and trigger culling, effectively removes it from the scene.
        //***DOESN'T WORK WITHOUT PHYSICS *** (won't crash, just won't do anything else)
        void setToCull();
        
        void import(const WorldObject *objPtr);
        void import(unsigned char *bufPtr);

        virtual void draw();
        virtual void update(int elapsedTime);
        virtual void think(int elapsedTime);
        virtual void interpolate(int elapsedTime);

        int makeBinStream(unsigned char *bufPtr);
        int fromBinStream(unsigned char *bufPtr);
        virtual void print();

        // Default Functions
        unsigned int getID() {return id;}
        unsigned int getPlayerID() {return playerid;}
        unsigned int getBldgID() {return bldgid;}

        Vector       getPosition()    {return position;}
        Vector       getVelocity()    {return velocity;}
        Vector       getForce()       {return force;}
        Quaternion   getOrientation() {return orientation;}
        ObjectType   getType()        {return type;}
        PhysicsState getPhysics()     {return physics;}
        uint64_t     getTimeStamp()   {return timestamp;}

        void setID      (unsigned int newid)       { id = newid; }
        void setPlayerID(unsigned int newplayerid) { playerid = newplayerid;}
        void setBldgID  (unsigned int newbldgid)   { bldgid = newbldgid;}

        void setPosition   (Vector newposition)        {position = newposition;}
        void setVelocity   (Vector newvelocity)        {velocity = newvelocity;}
        void setForce      (Vector newforce)           {force = newforce;}
        void setOrientation(Quaternion neworientation) {orientation = neworientation;}
        void setType       (ObjectType newtype)        {type = newtype;}
        void setPhysics    (PhysicsState newphysics)   {physics = newphysics;}
        void setTimeStamp  (time_t newTimeStamp)       {timestamp = (uint64_t)newTimeStamp;}
};

#endif
