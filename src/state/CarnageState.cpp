#include <map>
#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "CarnageState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"
#include "PhysObj.h"

#define ANGLE_SPEED 2
#define MOUSE_SPEED 1
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
   glutEntryFunc(&io::mouse_window);
   
   camera.eye = Vector( 0.0f, 30.0f, 30.0f);
   camera.lookAt = Vector( 0.0f, 0.0f, 0.0f);
   mouse_buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
   warp_mouse = true;

   // TODO Test Code
   WorldObject tmpObj = WorldObject(0,0,enumeration::DUMMY_SPHERE);
   tmpObj.setPosition(Vector(-25,1,0));
   tmpObj.setVelocity(Vector(10,25,0));
   tmpObj.setForce(Vector(0,-8,0));
   objects.push_back(tmpObj);

   tmpObj.setPosition(Vector(-25,1,0));
   tmpObj.setVelocity(Vector(5,25,0));
   tmpObj.setForce(Vector(0,-8,0));
   objects.push_back(tmpObj);
   // END Test Code

   // TODO BEGIN SECTION TO BE REMOVED
   physics.initPhysics();
   physics.loadFromFile("resources/test.lvl");
   // END REMOVE
}

void CarnageState::update(long milli_time) {
   ammo_recharge -= milli_time;
   updateInput(milli_time);
}

void CarnageState::updateInput(long milli_time) {
   // General Keyboard Layout
   if(keys[27])
	  exit(0);

   // Orbit Controls
   float deltaLocTilt = 0;
   float deltaLocTurn = 0;
   if(keys['w']) {
	   deltaLocTilt += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(keys['s']) {
	   deltaLocTilt -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(keys['a']) {
	   deltaLocTurn -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(keys['d']) {
	   deltaLocTurn += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(deltaLocTurn || deltaLocTilt) {
	   camera.phiMaxAngle = M_PI-0.14;
	   camera.phiMinAngle = 0.0f;
	   camera.eye = camera.orbitNode(Vector(0,0,0), camera.eye, deltaLocTurn, deltaLocTilt);
   }

   // Freelook Controls
   float deltaCamTilt = 0;
   float deltaCamTurn = 0;

   if(warp_mouse) {
	   glutWarpPointer(global::width >> 1, global::height >> 1);
   }
   if(mouse_x != 0) {
	   deltaCamTurn =  mouse_x * MOUSE_SPEED * (milli_time / 1000.0f);
	   mouse_x = 0;
   }
   if(mouse_y != 0) {
   	   deltaCamTilt = -mouse_y * MOUSE_SPEED * (milli_time / 1000.0f);
   	   mouse_y = 0;
   }
   if(special_keys[GLUT_KEY_UP]) {
	   deltaCamTilt += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(special_keys[GLUT_KEY_DOWN]) {
	   deltaCamTilt -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(special_keys[GLUT_KEY_LEFT]) {
	   deltaCamTurn -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(special_keys[GLUT_KEY_RIGHT]) {
	   deltaCamTurn += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(deltaCamTurn || deltaCamTilt) {
	   //printf("deltaCamTurn=%4.6f deltaCamTilt=%4.6f\n", deltaCamTurn, deltaCamTilt);
	   camera.inverted = 1;
	   camera.phiMaxAngle = M_PI;
	   camera.phiMinAngle =-M_PI;
	   camera.rotateView(deltaCamTurn, deltaCamTilt);
   }

   // FIRE CONTROLS
   if((keys[' '] || mouse_buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN) && ammo_recharge <= 0) {
	  Vector dir = (camera.eye-camera.lookAt) * -1;
	  dir.norm();

	  DummyAmmoUnit ammo = DummyAmmoUnit();
	  ammo.setPosition(camera.eye);
	  ammo.setVelocity(dir * 50);
	  ((CarnageState*)global::stateManager.currentState)->physics.addAmmo(ammo);

	  ammo_recharge = RECHARGE_TIME;
   }
}




