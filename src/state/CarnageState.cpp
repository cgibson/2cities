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

#define RECHARGE_TIME 200

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
   // TODO END DEMO CODE

   // General Keyboard Layout
   if(io::keys[27]) {
	  networkManager->network->disconnectServer();
      exit(0);
   }

   // FIRE CONTROLS
   if((io::keys[' '] || io::mouse_buttons[MOUSE_LEFT] == GLUT_DOWN) && ammo_recharge <= 0) {
	  //Vector dir = (camera.eye-camera.lookAt) * -1;
	  //dir.norm();
	  // uniqueID creation & addObject(WorldObject)
	  static int newObjID = 10000;
	  WorldObject newObj = WorldObject(newObjID++,0,enumeration::DUMMY_CONE);
	  newObj.setPosition(camera->position() - Vector( 0.0f, 1.0f, 0.0f));
	  //newObj.setVelocity(dir * 50);
	  newObj.setVelocity(camera->viewVec() * 50);
/*	  Vector strafe = camera->strafeVec();
	  float angle = acos(Vector (0, 0, -1) * camera->viewVec()) * 180 / 3.14159;
	  if (camera->liftVec() * Vector(0,1,0) < 0)
	  {
	    angle *= -1;
	  }
	  Quaternion quat;
	  quat.setAxis(strafe);
	  quat.setAngle(angle);
	  newObj.setOrientation(quat);
*/	  networkManager->network->addObject(newObj);

	  ammo_recharge = RECHARGE_TIME;
   }
#endif
}




