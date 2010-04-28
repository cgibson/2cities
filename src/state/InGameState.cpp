#include <map>
#include <GL/glut.h>

#include "InGameState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

using namespace io;

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
   if(keys['w'])
      global::camera.eye += Vector( 1, 0, 0);

   if(keys['s'])
      global::camera.eye += Vector(-1, 0, 0);

   if(keys['d'])
      global::camera.eye += Vector( 0, 0, 1);

   if(keys['a'])
      global::camera.eye += Vector( 0, 0,-1);

   if(keys[27])
      exit(0);

   // TODO Mouse Control Items


}

