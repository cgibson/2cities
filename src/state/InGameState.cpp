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
#include "../helper/camera.h"
#include "../helper/Vector.h"
#include "PhysObj.h"

#define ANGLE_SPEED 2
#define DIST_SPEED 10
#define PI 3.1415
#define DIST 30

using namespace io;
using namespace global;

InGameState::InGameState() {
   theta = 3.0 * PI / 4.0f;
   distance = DIST;
   phi = PI / 5.0f;
   initialize();
}

InGameState::~InGameState() {}

void InGameState::initialize() {
   // Keyboard Callbacks
   glutKeyboardFunc(&io::key_down);
   glutKeyboardUpFunc(&io::key_up);
   glutSpecialFunc(&io::special_key_down);
   glutSpecialUpFunc(&io::special_key_up);

   // Mouse Callbacks
   glutMouseFunc(&io::mouse_click);
   glutMotionFunc(&io::mouse_motion);
   glutPassiveMotionFunc(&io::mouse_motion);
   //glutSetCursor(GLUT_CURSOR_NONE);

   camera.eye = Vector( 0.0f, 30.0f, 30.0f);
   camera.lookAt = Vector( 0.0f, 0.0f, 0.0f);
}

void InGameState::update(long milli_time) {
   updateInput(milli_time);
}

void InGameState::updateInput(long milli_time) {
   // General Keyboard Layout

   // Freelook Controls
   float deltaTilt = 0;
   float deltaTurn = 0;
   if(special_keys[GLUT_KEY_UP]) {
	   deltaTilt += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(special_keys[GLUT_KEY_DOWN]) {
	   deltaTilt -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(special_keys[GLUT_KEY_LEFT]) {
	   deltaTurn -= ANGLE_SPEED * (milli_time / 1000.0f);
   }
   if(special_keys[GLUT_KEY_RIGHT]) {
	   deltaTurn += ANGLE_SPEED * (milli_time / 1000.0f);
   }
   camera.phiMaxAngle = M_PI;
   camera.phiMinAngle = 0.0f;
   if(deltaTurn || deltaTilt)
	   camera.rotateView(deltaTurn, deltaTilt);

   // Strafe Controls
   float deltaFwd  = 0;
   float deltaSide = 0;
   float deltaUp   = 0;
   if(keys['w']) {
	   deltaFwd += 0.5f;
   }
   if(keys['s']) {
   	   deltaFwd -= 0.5f;
   }
   if(keys['d']) {
   	   deltaSide += 0.5f;
   }
   if(keys['a']) {
   	   deltaSide -= 0.5f;
   }
   camera.strafe(deltaFwd, deltaSide, deltaUp);
   
   if(keys['f']) {
     distance += DIST_SPEED * (milli_time / 1000.0f);
   }
   
   if(keys['r']) {
     distance -= DIST_SPEED * (milli_time / 1000.0f);
   }

   // TODO Mouse Control Items


}

