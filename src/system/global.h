#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "../state/StateManager.h"
#include "../scene/factory.h"
#include "../helper/camera.h"

/*
 * All values within global namespace will be available for everyone
 */
class NetworkClient;

namespace global
{

  extern StateManager stateManager; // global state manager
  extern NetworkClient *network;
  extern ObjectFactory factory;
  
  extern int width;  // window width
  extern int height; // wondow height
  
  extern char* app_title;
  
  extern bool fullscreen;  // whether or not the window is fullscreen
  
  extern Camera camera;
  
  extern float fill_color[4];

  extern int fps; // frames per second (not necessarily *live*)
}

//#include "../network/NetworkClient.h"

#endif
