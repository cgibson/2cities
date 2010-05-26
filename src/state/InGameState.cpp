#include <map>
#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <math.h>

#include "InGameState.h"
#include "CarnageState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../graphics/camera.h"
#include "../helper/Vector.h"

#define ANGLE_SPEED 2
#define MOUSE_SPEED 1
#define STRAFE_SPEED 20
#define PI 3.1415
#define DIST 30

using namespace io;
using namespace global;

InGameState::InGameState() {
   theta = 3.0 * PI / 4.0f;
   distance = DIST;
   phi = PI / 5.0f;
   //initialize();
}

InGameState::~InGameState() {}

void InGameState::initialize() {
#ifdef CLIENT
	// i/o initializtion not done in the state
    // anymore, it's done globally at app launch
    // we just capture the mouse
    io::capture_mouse();

   //camera.eye = Vector( 0.0f, 30.0f, 30.0f);
   //camera.lookAt = Vector( 0.0f, 0.0f, 0.0f);
#endif
}

void InGameState::update(long milli_time) {
#ifdef CLIENT
   updateInput(milli_time);
#endif
}

void InGameState::updateInput(long milli_time) {
#ifdef CLIENT
   // General Keyboard Layout
	if(io::keys[27]) {
		exit(0);
	}

   if(io::keys['f']) {
	   if (io::captured) {
		   io::release_mouse();
	   } else {
		   io::capture_mouse();
	   }
   }

   // Strafe Controls
   float deltaFwd  = 0;
   float deltaSide = 0;
   float deltaUp   = 0;
   if(io::keys['w']) {
	   deltaFwd += STRAFE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['s']) {
	   deltaFwd -= STRAFE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['a']) {
	   deltaSide += STRAFE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['d']) {
	   deltaSide -= STRAFE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys[' ']) {
	   deltaUp += STRAFE_SPEED * (milli_time / 1000.0f);
   }
   if(io::keys['c']) {
	   deltaUp -= STRAFE_SPEED * (milli_time / 1000.0f);
   }
   if(deltaFwd || deltaSide || deltaUp) {
	   //camera.strafe(deltaFwd, deltaSide, deltaUp);
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
	   //camera.inverted = 1;
	   //camera.phiMaxAngle = M_PI;
	   //camera.phiMinAngle =-M_PI;
	   //camera.rotateView(deltaCamTurn, deltaCamTilt);
   }
#endif
}

