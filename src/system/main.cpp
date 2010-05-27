#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "gl_helper.h"
#include "io.h"
#include "../graphics/graphics.h"
#include "../state/CarnageState.h"
#include "../network/NetworkManager.h"
//#include "../scene/ModelManager.h"

int msLast = 0;
bool running;
E_STATE beginState;

void updateLoop()
{
    static int msFps = 0;
    static int frameCount = 0;

    // calculate milliseconds since last update
    int msNow = elapsed_ms();
    if (msLast == 0)
    {
        // bootstrap the first call
        msLast = msFps = msNow;
        return;
    }
    int elapsed = msNow - msLast;

    // update all modules
    io::update(elapsed);
    if (global::camera != NULL) global::camera->update(elapsed);
    gfx::update(elapsed);
    global::stateManager->currentState->update(elapsed);
    global::networkManager->network->update(elapsed);

    vector<WorldObject *> *objList = &global::stateManager->currentState->objects;
    vector<WorldObject *>::iterator objIt;
    for(objIt = objList->begin(); objIt != objList->end(); ++objIt)
    {
      (*objIt)->update(elapsed);
    }

    // update the fps counter (every quarter second)
    frameCount++;
    if (msNow - msFps > 250)
    {
        global::fps = frameCount * 4;
        frameCount = 0;
        msFps = msNow;
    }

    // update the last ms counter
    msLast = msNow;
}

void initState()
{
  global::stateManager->changeCurrentState(beginState);
  global::stateManager->initialize();
}

void initialize()
{

  global::width = 1024;
  global::height = 768;
  global::app_title = (char*)malloc(sizeof(char) * 80);
  sprintf(global::app_title, "2Cities : The Game");

  // initialize SDL (timer and sound in the future)
  if (SDL_Init(SDL_INIT_TIMER) < 0)
  {
    fprintf(stderr, "Oh noes! Failed to initialize SDL! Sadface!\n");
    exit(EXIT_FAILURE);
  }
  atexit(SDL_Quit);

  gl::init();

  // initialization of all other modules
  io::init();
  gfx::init();

  initState();

  global::networkManager->initialize();
  //global::modelManager->initialize();

  glutIdleFunc(updateLoop);

  glutMainLoop();
}

/*
 * application kickstart.
 */
int main(int argc, char** argv)
{
	beginState = CARNAGE_STATE;
	global::fullscreen = false;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-build") == 0)
		{
			beginState = BUILD_STATE;
			gfx::draw_axis = true;
		}
		else if (strcmp(argv[i], "-fullscreen") == 0)
		{
			global::fullscreen = true;
		}
	}

  glutInit(&argc, argv);
  initialize();
  return 0;
}
