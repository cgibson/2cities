#include "global.h"

#ifdef CLIENT
	#include "../graphics/camera.h"
	#include "../scene/ModelManager.h"
#endif

/*
 * All values within global namespace will be available for everyone
 */

namespace global
{
  StateManager  *stateManager    = new StateManager();
  ObjectFactory  *factory        = new ObjectFactory();
  NetworkManager *networkManager = new NetworkManager();
#ifdef CLIENT
//  ModelManager   *modelManager   = new ModelManager();
#endif

  int width;  // window width
  int height; // wondow height

  char* app_title;

  bool fullscreen;  // whether or not the window is fullscreen

#ifdef CLIENT
  Camera *camera = NULL;
#endif

  float fill_color[4] = {0.05f, 0.05f, 0.05f, 0.05f};

  int fps;
  int pbs_recv;
  int pbs_sent;

  int elapsed_ms()
  {
    return (int)SDL_GetTicks();
  }
}
