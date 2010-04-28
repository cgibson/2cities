#include <map>
#include <GL/glut.h>

#include "CarnageState.h"
#include "PhysObj.h"
#include "../system/io.h"

#include "../system/global.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

using namespace io;

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
/*
   glutMouseFunc(&CarnageState::mouse_click);
   glutMotionFunc(&CarnageState::mouse_motion);
   glutPassiveMotionFunc(&CarnageState::mouse_motion);
   glutSetCursor(GLUT_CURSOR_NONE);
*/

   // Temp DEV hard coded world
   DummyBuildingUnit *dummyBlock;
   // 25 blocks in square layout hovering 1 unit above ground place. (mass = 1, size = 1)
   for(int i = 0; i < 100; i+=2) {
      dummyBlock = new DummyBuildingUnit(Vector(i/10,1,i%10),Vector(0,0,0), 0.0f, 1.0f, 1.0f);
      bunits.push_back (*dummyBlock);
   }
}

// Temp inherit from InGameState's generic controls
// void CarnageState::update(int val) {}

void CarnageState::mouse_click(int button, int state, int x, int y) {

}

void CarnageState::mouse_motion(int x, int y) {

}

