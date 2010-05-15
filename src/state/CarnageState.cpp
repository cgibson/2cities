#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "CarnageState.h"

#include <map>
#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

#define ANGLE_SPEED 2
#define MOUSE_SPEED 1
#define DIST_SPEED 10
#define PI 3.1415
#define DIST 30
#define RECHARGE_TIME 200

using namespace global;

CarnageState::CarnageState() {
	initialize();
}

CarnageState::~CarnageState() {}

void CarnageState::initialize() {
#ifdef CLIENT
	// i/o initializtion not done in the state
    // anymore, it's done globally at app launch
    // we just capture the mouse
    io::capture_mouse();

   camera.eye = Vector( 0.0f, 30.0f, 30.0f);
   camera.lookAt = Vector( 0.0f, 0.0f, 0.0f);
#endif

   networkManager->network->loadLevel("resources/test.lvl");
}

void CarnageState::update(long milli_time) {
#ifdef CLIENT
	ammo_recharge -= milli_time;
   updateInput(milli_time);
#endif
}

void CarnageState::updateInput(long milli_time) {
#ifdef CLIENT
   // General Keyboard Layout
   if(io::keys[27]) {
      exit(0);
   }
   if(io::keys['l']) {
	   networkManager->network->loadLevel("resources/test.lvl");
   }
   // Orbit Controls
   float deltaLocTilt = 0;
   float deltaLocTurn = 0;
   if(io::keys['w']) {
	   deltaLocTilt += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['s']) {
	   deltaLocTilt -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['a']) {
	   deltaLocTurn -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['d']) {
	   deltaLocTurn += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(deltaLocTurn || deltaLocTilt) {
	   camera.phiMaxAngle = M_PI-0.2;
	   camera.phiMinAngle = 0.0f;
	   camera.eye = camera.orbitNode(Vector(0,0,0), camera.eye, deltaLocTurn, deltaLocTilt);
   }

   // Freelook Controls
   float deltaCamTilt = 0;
   float deltaCamTurn = 0;

    // only update the view if the mouse is captured
    if (io::captured)
    {
	    deltaCamTurn =  io::mouse_x * MOUSE_SPEED * (milli_time / 1000.0f);
   	    deltaCamTilt = -io::mouse_y * MOUSE_SPEED * (milli_time / 1000.0f);
    }
   if(io::special_keys[GLUT_KEY_UP]) {
	   deltaCamTilt += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(io::special_keys[GLUT_KEY_DOWN]) {
	   deltaCamTilt -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(io::special_keys[GLUT_KEY_LEFT]) {
	   deltaCamTurn -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(io::special_keys[GLUT_KEY_RIGHT]) {
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
   if((io::keys[' '] || io::mouse_buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN) && ammo_recharge <= 0) {
	  Vector dir = (camera.eye-camera.lookAt) * -1;
	  dir.norm();

	  // TODO uniqueID creation & addObject(WorldObject)
	  static int newObjID = 10000;
	  WorldObject newObj = WorldObject(newObjID++,0,enumeration::DUMMY_SPHERE);
	  newObj.setPosition(camera.eye);
	  newObj.setVelocity(dir * 50);
	  networkManager->network->addObject(newObj);

	  ammo_recharge = RECHARGE_TIME;
   }
#endif
}




