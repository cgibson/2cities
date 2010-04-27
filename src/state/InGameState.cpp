#include <map>
#include <GL/glut.h>

#include "InGameState.h"

#include "../system/global.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

using namespace global;

InGameState::InGameState() {
   initialize();
}

InGameState::~InGameState() {}

void InGameState::initialize() {
   // Keyboard Callbacks
   glutKeyboardFunc(&global::key_down);
   glutKeyboardUpFunc(&global::key_up);
   glutSpecialFunc(&global::special_key_down);
   glutSpecialUpFunc(&global::special_key_up);

   // Mouse Callbacks
/*
   glutMouseFunc(&InGameState::mouse_click);
   glutMotionFunc(&InGameState::mouse_motion);
   glutPassiveMotionFunc(&InGameState::mouse_motion);
   glutSetCursor(GLUT_CURSOR_NONE);
*/
}

void InGameState::update() {
   if(keys['w'])
      global::camera.eye = Vector(global::camera.eye.x()+1,global::camera.eye.y(),global::camera.eye.z());

   if(keys['s'])
      global::camera.eye = Vector(global::camera.eye.x()-1,global::camera.eye.y(),global::camera.eye.z());

   if(keys['d'])
      global::camera.eye = Vector(global::camera.eye.x(),global::camera.eye.y(),global::camera.eye.z()+1);

   if(keys['a'])
      global::camera.eye = Vector(global::camera.eye.x(),global::camera.eye.y(),global::camera.eye.z()-1);

   if(keys[27])
      exit(0);
}

void InGameState::mouse_click(int button, int state, int x, int y) {

}

void InGameState::mouse_motion(int x, int y) {

}

