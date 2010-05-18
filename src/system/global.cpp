#include "global.h"

#include "../network/NetworkManager.h"

/*
 * All values within global namespace will be available for everyone
 */
 
namespace global
{
  StateManager *stateManager = new StateManager(); // global state manager
  ObjectFactory *factory = new ObjectFactory();
  NetworkManager *networkManager = new NetworkManager();

  int width;  // window width
  int height; // wondow height
  
  char* app_title;
  
  bool fullscreen;  // whether or not the window is fullscreen
  
  Camera camera;
  
  float fill_color[4] = {0.05f, 0.05f, 0.05f, 0.05f};

  int fps;
  int pbs_recv;
  int pbs_sent;

  int elapsed_ms()
  {
    return (int)SDL_GetTicks();
  }
}
