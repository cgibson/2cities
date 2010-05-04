#ifndef _AMMOUNIT_H_
#define _AMMOUNIT_H_

#include "WorldObject.h"
#include "../system/enum.h"

#include "../helper/Vector.h"

using namespace enumeration;

#define AMMO_MASS 1
#define AMMO_RADIUS 1
#define UNIT_SHAPE SMALL_SPHERE

/**
 * AmmoUnit class
 */
class AmmoUnit: public WorldObject
{
    protected:
		virtual void draw() {};
		virtual void update(int elapsedTime) {};
		virtual void think(int elapsedTime) {};

    public:
        virtual void AmmoUnit();
        virtual void AmmoUnit(unsigned int newid, unsigned int newplayerid);
};

#endif
