#include <map>
#include <GL/glut.h>
#include <math.h>

#include "InGameState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

#define ANGLE_SPEED 2
#define DIST_SPEED 10
#define PI 3.1415
#define DIST 20

const float BUTTON_ROTATE = 0.1f;

using namespace io;
using namespace global;

InGameState::InGameState() {
   theta = 0;
   distance = DIST;
   phi = PI / 4.0f;
   initialize();
   updateCamera();
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
}

void InGameState::update(long milli_time) {
   updateInput(milli_time);
}

void InGameState::updateCamera()
{
  camera.lookAt = Vector(0,0,0);
  camera.eye = Vector(
                   sin(theta) * distance * cos(phi),
                   distance * sin(phi),
                   cos(theta) * distance * cos(phi)
               );
}

void InGameState::updateInput(long milli_time) {
   // General Keyboard Layout
  
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
   
   updateCamera();

   if(keys[27])
      exit(0);

   // TODO Mouse Control Items


}

