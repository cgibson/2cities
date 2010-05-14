#include "global.h"
#ifdef CLIENT
  #include "../network/NetworkPrivate.h"
  #include "../network/NetworkClient.h"
#else
  #include "../network/NetworkServer.h"
#endif
/*
 * All values within global namespace will be available for everyone
 */
 
namespace global
{
  StateManager *stateManager = new StateManager(); // global state manager
  ObjectFactory *factory = new ObjectFactory();
#ifdef CLIENT
  NetworkSystem *network = new NetworkPrivate();
  //NetworkSystem *network = new NetworkClient();
#else
  NetworkSystem *network = new NetworkServer();
#endif
  
  int width;  // window width
  int height; // wondow height
  
  char* app_title;
  
  bool fullscreen;  // whether or not the window is fullscreen
  
  Camera camera;
  
  float fill_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

  int fps;
}
