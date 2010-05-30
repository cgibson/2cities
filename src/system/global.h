#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <SDL/SDL.h>
#include "../state/StateManager.h"
#include "../scene/factory.h"

// forward declaration
class Camera;

/*
 * All values within global namespace will be available for everyone
 */
class ModelManager;
class NetworkManager;
class SoundManager;

namespace global
{
  extern StateManager   *stateManager;
  extern ObjectFactory  *factory;
#ifdef CLIENT
  extern ModelManager   *modelManager;
  extern SoundManager	*soundManager;
#endif
  extern NetworkManager *networkManager;

  extern int width;  // window width
  extern int height; // wondow height

  extern char* app_title;

  extern bool fullscreen;  // whether or not the window is fullscreen

  extern Camera *camera;

  extern float fill_color[4];

  extern int fps; // frames per second (not necessarily *live*)
  extern int pbs_recv;
  extern int pbs_sent;
  extern bool interpolateObjects;

  // call this to get the number of milliseconds elapsed since the program started
  int elapsed_ms();
}

#ifdef CLIENT
	#include "../audio/SoundManager.h"
	#include "../scene/ModelManager.h"
#endif
#include "../network/NetworkManager.h"
#endif
