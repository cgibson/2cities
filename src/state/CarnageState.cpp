#include <map>
#include <GL/glut.h>

#include "CarnageState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"
#include "PhysObj.h"

#define ANGLE_SPEED 2
#define DIST_SPEED 10
#define PI 3.1415
#define DIST 30
#define RECHARGE_TIME 200

using namespace io;
using namespace global;

CarnageState::CarnageState() {
   initialize();
}

CarnageState::~CarnageState() {}

void CarnageState::initialize() {
   // Keyboard Callbacks
   glutKeyboardFunc(&io::key_down);
   glutKeyboardUpFunc(&io::key_up);
   glutSpecialFunc(&io::special_key_down);
   glutSpecialUpFunc(&io::special_key_up);

   // Mouse Callbacks
   glutMouseFunc(&io::mouse_click);
   glutMotionFunc(&io::mouse_motion);
   glutPassiveMotionFunc(&io::mouse_motion);
   glutSetCursor(GLUT_CURSOR_NONE);
   
   physics.initPhysics();

   // TODO // Temp DEV hard coded world
   physics.loadFromFile("resources/test.lvl");
   // TODO END // TEMP DEV CODE
}

void CarnageState::update(long milli_time) {
   ammo_recharge -= milli_time;
   updateInput(milli_time);
}

void CarnageState::updateCamera() {
   camera.lookAt = Vector(0,0,0);
   camera.eye = Vector(
				   sin(theta) * distance * cos(phi),
				   distance * sin(phi),
				   cos(theta) * distance * cos(phi)
			   );
}

void CarnageState::updateInput(long milli_time) {
   // General Keyboard Layout
   if(keys[27])
	  exit(0);

   if(keys['w']) {
	  phi += ANGLE_SPEED * (milli_time / 1000.0f);
   }

   if(keys['s']) {
	  phi -= ANGLE_SPEED * (milli_time / 1000.0f);
   }

   if(keys['a']) {
	  theta -= ANGLE_SPEED * (milli_time / 1000.0f);
   }

   if(keys['d']) {
	  theta += ANGLE_SPEED * (milli_time / 1000.0f);
   }

   if(keys['f']) {
	 distance += DIST_SPEED * (milli_time / 1000.0f);
   }

   if(keys['r']) {
	 distance -= DIST_SPEED * (milli_time / 1000.0f);
   }

   if(keys[' '] && ammo_recharge <= 0) {
	  Vector camera_pos = Vector(
						   sin(theta) * distance * cos(phi),
						   distance * sin(phi),
						   cos(theta) * distance * cos(phi)
						  );
	  Vector dir = camera_pos * -1 + Vector(0,4,0);
	  dir.norm();
	  DummyAmmoUnit ammo = DummyAmmoUnit();
	  ammo.setPosition(camera_pos);
	  ammo.setVelocity(dir * 50);
	  ((CarnageState*)global::stateManager.currentState)->physics.addAmmo(ammo);

	  ammo_recharge = RECHARGE_TIME;
   }

   updateCamera();

   // TODO Mouse Control Items
}




