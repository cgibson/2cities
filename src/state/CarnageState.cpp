#include <map>
#include <GL/glut.h>

#include "CarnageState.h"
#include "PhysObj.h"

#include "../system/global.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

using namespace global;

CarnageState::CarnageState() {
   initialize();
}

CarnageState::~CarnageState() {}

void CarnageState::initialize() {
   // Keyboard Callbacks
   glutKeyboardFunc(&global::key_down);
   glutKeyboardUpFunc(&global::key_up);
   glutSpecialFunc(&global::special_key_down);
   glutSpecialUpFunc(&global::special_key_up);

   // Mouse Callbacks
/*
   glutMouseFunc(&CarnageState::mouse_click);
   glutMotionFunc(&CarnageState::mouse_motion);
   glutPassiveMotionFunc(&CarnageState::mouse_motion);
   glutSetCursor(GLUT_CURSOR_NONE);
*/

   // Temp DEV hard coded world
   //BuildingUnit *dummyBlock = new BuildingUnit();
   //bunits.push_back (*dummyBlock);
}

// Temp inherit from InGameState's generic controls
/*
void CarnageState::update() {
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
*/

void CarnageState::mouse_click(int button, int state, int x, int y) {

}

void CarnageState::mouse_motion(int x, int y) {

}

