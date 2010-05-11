#include <map>
#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <math.h>

#include "BuildState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"
#include "../graphics/graphics.h"

using namespace io;
using namespace global;

BuildState::BuildState() {
   initialize();
}

BuildState::~BuildState() {}

void BuildState::initialize() {
    // i/o initializtion not done in the state
    // anymore, it's done globally at app launch
    // we just capture the mouse
    //io::capture_mouse();
   io::release_mouse();
   camera.eye = Vector( 0.0f, 30.0f, 30.0f);
   camera.lookAt = Vector( 0.0f, 0.0f, 0.0f);
}

void BuildState::update(long milli_time) {
	BuildState::updateInput(milli_time);
	//gfx::hud.console.info("Hello, BuildState!");
}

void BuildState::updateInput(long milli_time) {
  
	if(io::mouse_buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
	{
	  gfx::hud.console.info("Hello, Left Mouse at points %d, %d!", io::mouse_x, io::mouse_y);
	}
}
