#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "CarnageState.h"

#include <map>
#include "../system/global.h"
#include "../system/io.h"
#include "../graphics/orbitalcamera.h"
#include "../helper/Vector.h"
#include "../scene/CustomObject.h"

using namespace global;

const int CarnageState::RECHARGE_TIME = 1000; // in milliseconds

CarnageState::CarnageState() {
	//initialize();
}

CarnageState::~CarnageState() {}

void CarnageState::initialize() {
#ifdef CLIENT
	// i/o initializtion not done in the state
    // anymore, it's done globally at app launch
    // we just capture the mouse
    io::capture_mouse();
    ammo_recharge = 0;
    ammo_type = FRACTAL_BOMB;

   // initialize our camera (orbital)
   if (global::camera != NULL) delete global::camera;
   global::camera = new OrbitalCamera();
   global::camera->init(Vector(30.0, 30.0, 30.0), Vector(0.0, 5.0, 0.0));
#endif
}

void CarnageState::update(long milli_time) {
#ifdef CLIENT
	ammo_recharge -= milli_time;
	ammo_recharge = (ammo_recharge < 0) ? 0 : ammo_recharge; // clamp to 0
   updateInput(milli_time);
#endif
}

void CarnageState::updateInput(long milli_time) {
#ifdef CLIENT
   // TODO REMOVE DEMO CODE
   if(io::keys['8']) {
	  io::keys['8'] = false;
      networkManager->network->loadLevel("resources/2base.lvl");
   }
   if(io::keys['9']) {
	  io::keys['9'] = false;
      networkManager->network->loadLevel("resources/test2.lvl");
   }
   if(io::keys['0']) {
	  io::keys['0'] = false;
      networkManager->network->loadLevel("resources/bigplus.lvl");
   }
   if(io::keys['-']) {
	  io::keys['-'] = false;
      networkManager->network->loadLevel("resources/ibigplus.lvl");
   }
   if(io::keys['c']) {
	  io::keys['c'] = false;
	  networkManager->network->serverConnect("127.0.0.1", 5060);
   }
   if (io::keys[';']) {
	   io::keys[';'] = false;
	   ShockwaveEffect *fx = new ShockwaveEffect();
	   fx->init(4000, 50.0);
	   fx->position(Vector(0.0, 5.0, 0.0));
	   gfx::fxManager.addEffect(fx);
   }
   // TODO END DEMO CODE

   // General Keyboard Layout
   if(io::keys[27]) {
	  networkManager->network->serverDisconnect();
      exit(0);
   }

   // FIRE CONTROLS
   if((io::keys[' '] || io::mouse_buttons[MOUSE_LEFT] == GLUT_DOWN) && ammo_recharge <= 0) {
	  WorldObject *newObjPtr = global::factory->makeObject(ammo_type);
	  newObjPtr->setPosition(camera->position() - Vector( 0.0f, 1.0f, 0.0f));
	  newObjPtr->setVelocity((camera->viewVec() + Vector(0.0f, 0.15f, 0.0f)) * 50); // offset the view vector a bit to more closely match the targeting reticle
	  networkManager->network->addObject(newObjPtr);

	  ammo_recharge = RECHARGE_TIME;
   }
#endif
}




