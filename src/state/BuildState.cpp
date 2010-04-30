#include <map>
#include <GL/glut.h>
#include <math.h>

#include "BuildState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"
#include "PhysObj.h"

using namespace io;
using namespace global;

BuildState::BuildState() {
   initialize();
}

BuildState::~BuildState() {}

void BuildState::initialize() {
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

/* TODO Inherit from InGameState
void BuildState::update(long milli_time) {
	BuildState::updateInput(milli_time);
}
*/
