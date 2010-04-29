#include <map>
#include <GL/glut.h>
#include <math.h>

#include "InGameState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

const float BUTTON_ROTATE = 0.1f;

using namespace io;
using namespace global;

InGameState::InGameState() {
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
}

void InGameState::update(long milli_time) {
   updateInput(milli_time);
}

void InGameState::updateInput(long milli_time) {
   // General Keyboard Layout
   if(keys['w']) {
      camera.eye    = camera.eye    + Vector(-sin(camera.lookAt.y()), 0,-cos(camera.lookAt.y()));
//      camera.lookAt = camera.lookAt + Vector(-sin(camera.lookAt.y()), 0,-cos(camera.lookAt.y()));
   }

   if(keys['s']) {
      camera.eye    = camera.eye    + Vector( sin(camera.lookAt.y()), 0, cos(camera.lookAt.y()));
//      camera.lookAt = camera.lookAt + Vector( sin(camera.lookAt.y()), 0, cos(camera.lookAt.y()));
   }

   if(keys['a']) {
      camera.eye    = camera.eye    + Vector(-cos(camera.lookAt.y()), 0, sin(camera.lookAt.y()));
      camera.lookAt = camera.lookAt + Vector(-cos(camera.lookAt.y()), 0, sin(camera.lookAt.y()));
   }

   if(keys['d']) {
      camera.eye    = camera.eye    + Vector( cos(camera.lookAt.y()), 0,-sin(camera.lookAt.y()));
      camera.lookAt = camera.lookAt + Vector( cos(camera.lookAt.y()), 0,-sin(camera.lookAt.y()));
   }

   if(keys[' ']) {
      camera.eye    = camera.eye    + Vector( 0, 1, 0);
      camera.lookAt = camera.lookAt + Vector( 0, 1, 0);
   }

   if(keys['c']) {
      camera.eye    = camera.eye    + Vector( 0,-1, 0);
      camera.lookAt = camera.lookAt + Vector( 0,-1, 0);
   }

   if(special_keys[GLUT_KEY_UP]) {
      camera.lookAt = camera.lookAt + Vector( 0, 1, 0);
   }

   if(special_keys[GLUT_KEY_DOWN]) {
      camera.lookAt = camera.lookAt + Vector( 0,-1, 0);
   }

   if(special_keys[GLUT_KEY_LEFT]) {
      camera.lookAt = camera.lookAt + Vector(-1, 0, 0);
   }

   if(special_keys[GLUT_KEY_RIGHT]) {
      camera.lookAt = camera.lookAt + Vector( 1, 0, 0);
   }

   if(keys[27])
      exit(0);

   // TODO Mouse Control Items


}

