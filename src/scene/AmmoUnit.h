#ifndef _AMMOUNIT_H_
#define _AMMOUNIT_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

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
	public:
        AmmoUnit();
        AmmoUnit(unsigned int newid, unsigned int newplayerid);

    protected:
		virtual void draw();
		virtual void update(int elapsedTime);
		virtual void think(int elapsedTime);
};

#endif
