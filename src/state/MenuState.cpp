
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

using namespace global;
using namespace enumeration;

const int MenuState::MUSIC_DELAY = 1000;

MenuState::MenuState()
{

}

MenuState::~MenuState() {}

void MenuState::initialize() {
#ifdef CLIENT
    realStateType = enumeration::MENU_STATE;

	// create the camera
	if (global::camera != NULL) delete global::camera;
	global::camera = new OrbitalCamera();
	global::camera->init(Vector(30.0, 30.0, 30.0), Vector(0.0, 5.0, 0.0));
	OrbitalCamera *cam = static_cast<OrbitalCamera *>(global::camera);
	cam->shiftCaptureMode(true);

	io::capture_mouse();

	// TODO: register io event hooks
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
		printf("Stopping the menu state music.\n");
		global::soundManager->stopPlayingMusic();
	}

	//play music for menu state
	if(io::keys[']'] && music_delay <= 0)
	{
		printf("Playing menu state music\n");
		global::soundManager->playMenuSong();
		music_delay = MUSIC_DELAY;
	}

   // TODO END DEMO CODE

#endif
}



