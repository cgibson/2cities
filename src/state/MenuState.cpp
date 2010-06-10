
#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "MenuState.h"

#include <map>
#include "../system/global.h"
#include "../system/enum.h"
#include "../system/io.h"
#ifdef CLIENT
#include "../graphics/graphics.h"
#endif

using namespace global;
using namespace enumeration;

const int MenuState::MUSIC_DELAY = 1000;

MenuState::MenuState()
{
	// nothing yet
}

MenuState::~MenuState() {}

void MenuState::initialize() {
#ifdef CLIENT
    realStateType = enumeration::MENU_STATE;

	// create the camera
	if (global::camera != NULL) delete global::camera;
	global::camera = new CinematicCamera();
	global::camera->init(Vector(30.0, 15.0, 30.0), Vector(0.0, 10.0, 0.0));

	io::capture_mouse();
#endif
}

void MenuState::update(long milli_time) {
#ifdef CLIENT
	music_delay -= milli_time;
	music_delay = (music_delay < 0) ? 0 : music_delay; // clamp to 0
	updateInput(milli_time);
#endif
}

void MenuState::updateInput(long milli_time) {
#ifdef CLIENT
   // TODO REMOVE DEMO CODE

	//stop music
	if(io::keys['['])
	{
		global::soundManager->stopPlayingMusic();
	}

	//play music for menu state
	if(io::keys[']'] && music_delay <= 0)
	{
		global::soundManager->playMenuSong();
		music_delay = MUSIC_DELAY;
	}
	
	if(io::keys['n'] && music_delay <= 0)
	{
		global::soundManager->playNextMenuSong();
		music_delay = MUSIC_DELAY;
	}

   // TODO END DEMO CODE

#endif
}



