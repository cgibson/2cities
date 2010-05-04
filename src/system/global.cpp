#include "global.h"
#include "../network/NetworkClient.h"
/*
 * All values within global namespace will be available for everyone
 */
 
namespace global
{

  StateManager stateManager; // global state manager
  NetworkClient *network;
  ObjectFactory factory;
  
  int width;  // window width
  int height; // wondow height
  
  char* app_title;
  
  bool fullscreen;  // whether or not the window is fullscreen
  
  Camera camera;
  
  float fill_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

  int fps;
}
