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
#include "../system/enum.h"

using namespace global;
using namespace enumeration;

const int CarnageState::RECHARGE_TIME = 1000; // in milliseconds
const int CarnageState::MUSIC_DELAY = 1000;

CarnageState::CarnageState() {
	cameraSetupComplete = false;
}

CarnageState::~CarnageState() {}

void CarnageState::initialize() {
#ifdef CLIENT
	// i/o initializtion not done in the state
    // anymore, it's done globally at app launch
    // we just capture the mouse
    io::capture_mouse();
    ammo_recharge = 0;
    music_delay = 0;
    ammo_type = FRACTAL_BOMB;
    
    oppPos = Vector(0,0,0);
    oppView = Vector(0,0,0);

   // initialize our camera (orbital)
   if (global::camera != NULL) delete global::camera;
   global::camera = new OrbitalCamera();
   global::camera->init(Vector(30.0, 30.0, 30.0), Vector(0.0, 5.0, 0.0));
   
   // initialize opponent vehicle
   opponent = global::factory->makeObject(RECOGNIZER);
	 opponent->setPosition(Vector(0, 0, 0));
	 opponent->setVelocity(Vector(0, 0, 0));
	 
   //opponentCannon = new ComplexObject(SMOOTH_CANNON);
   //playerCannon = new ComplexObject(SMOOTH_CANNON);
   playerCannon = global::factory->makeObject(SMOOTH);
	 playerCannon->setPosition(Vector(0, 0, 0));
	 playerCannon->setVelocity(Vector(0, 0, 0));
	 
	 int id = global::networkManager->network->getMyPlayerID();
	 
	 if(id == 1)
	 {
		 playerMat = gfx::materials + RED_PLAYER;
		 opponentMat = gfx::materials + BLUE_PLAYER;
	 }else{
		 playerMat = gfx::materials + BLUE_PLAYER;
		 opponentMat = gfx::materials + RED_PLAYER;
	 }
	 
#endif

   realStateType = enumeration::CARNAGE_STATE;
}

void CarnageState::update(long milli_time) {
#ifdef CLIENT
	ammo_recharge -= milli_time;
	ammo_recharge = (ammo_recharge < 0) ? 0 : ammo_recharge; // clamp to 0
	music_delay -= milli_time;
	music_delay = (music_delay < 0) ? 0 : music_delay; // clamp to 0

	// keep the camera centered on the proper half of the playing field
#ifdef CLIENT
	int playerID = global::networkManager->network->getMyPlayerID();
	if (!cameraSetupComplete && (playerID == 1 || playerID == 2))
	{
		OrbitalCamera *cam = static_cast<OrbitalCamera *>(global::camera);
		if (playerID == 1) // red
		{
			// center on BLUE'S base! (positive x)
			cam->center(Vector(global::map_width / 4.0, 5.0, 0.0));
		}
		else // blue
		{
			// center on RED'S base! (negative x)
			cam->center(Vector(-global::map_width / 4.0, 5.0, 0.0));
		}
		cameraSetupComplete = true;
	}
	
	// update opponent and its cannon
	opponent->setPosition(oppPos);
	Vector dir = oppView;
	dir.norm();
	Quaternion quat = Quaternion::GenerateRotationFromDirectionVector(dir);
	opponent->setOrientation(quat);
#endif

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

	//stop music
	if(io::keys['['])
	{
		printf("Stopping the carnage state music.\n");
		global::soundManager->stopPlayingMusic();
	}

	//play music for carnage state
	if(io::keys[']'] && music_delay <= 0)
	{
		printf("Playing carnage state music\n");
		global::soundManager->playCarnageSong();
		music_delay = MUSIC_DELAY;
	}

	if(io::keys['n'] && music_delay <= 0)
	{
		printf("Playing the next carnage state song\n");
		global::soundManager->playNextCarnageSong();
		music_delay = MUSIC_DELAY;
	}

   // TODO END DEMO CODE

   // General Keyboard Layout
   if(io::keys[27]) {
	  networkManager->network->serverDisconnect();
      exit(0);
   }

   // FIRE CONTROLS
   if((io::keys[' '] || io::mouse_buttons[MOUSE_LEFT] == GLUT_DOWN) && ammo_recharge <= 0)
   {
	  global::soundManager->playCarnageSfx(0);
	  WorldObject *newObjPtr = global::factory->makeObject(SHOTGUN);//ammo_type);
	  newObjPtr->setPosition(camera->position() - Vector( 0.0f, 1.0f, 0.0f));
	  newObjPtr->setVelocity((camera->viewVec() + Vector(0.0f, 0.15f, 0.0f)) * 50); // offset the view vector a bit to more closely match the targeting reticle
	  networkManager->network->addObject(newObjPtr);
	  ammo_recharge = RECHARGE_TIME;
   }
#endif
}



