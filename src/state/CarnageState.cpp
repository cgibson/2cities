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

#define RECHARGE_TIME 300

using namespace global;

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
    ammo_type = DUMMY_SPHERE;

   // initialize our camera (orbital)
   if (global::camera != NULL) delete global::camera;
   global::camera = new OrbitalCamera();
   global::camera->init(Vector(30.0, 30.0, 30.0), Vector(0.0, 5.0, 0.0));
#endif
}

void CarnageState::update(long milli_time) {
#ifdef CLIENT
	ammo_recharge -= milli_time;
   updateInput(milli_time);
#endif
}

void CarnageState::updateInput(long milli_time) {
#ifdef CLIENT
   // TODO REMOVE DEMO CODE
   if(io::keys['1']) {
	  io::keys['1'] = false;
      networkManager->network->loadLevel("resources/2base.lvl");
   }
   if(io::keys['2']) {
	  io::keys['2'] = false;
      networkManager->network->loadLevel("resources/test2.lvl");
   }
   if(io::keys['3']) {
	  io::keys['3'] = false;
      networkManager->network->loadLevel("resources/bigplus.lvl");
   }
   if(io::keys['4']) {
	  io::keys['4'] = false;
      networkManager->network->loadLevel("resources/ibigplus.lvl");
   }
   if(io::keys['c']) {
	  io::keys['c'] = false;
	  networkManager->network->connectServer("127.0.0.1", 5060);
   }
   // TODO END DEMO CODE

   // General Keyboard Layout
   if(io::keys[27]) {
	  networkManager->network->disconnectServer();
      exit(0);
   }

   // FIRE CONTROLS
   if((io::keys[' '] || io::mouse_buttons[MOUSE_LEFT] == GLUT_DOWN) && ammo_recharge <= 0) {
	  WorldObject *newObjPtr = global::factory->makeObject(ammo_type);
	  newObjPtr->setPosition(camera->position() - Vector( 0.0f, 1.0f, 0.0f));
	  newObjPtr->setVelocity(camera->viewVec() * 50);
	  networkManager->network->addObject(newObjPtr);

	  ammo_recharge = RECHARGE_TIME;
   }
#endif
}




