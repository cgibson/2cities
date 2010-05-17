#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "gl_helper.h"
#include "io.h"
#include "../graphics/graphics.h"
#include "../state/CarnageState.h"
#include "../network/NetworkManager.h"

int msLast = 0;
bool running;
E_STATE beginState;

void updateLoop()
{
    static int msFps = 0;
    static int frameCount = 0;

    // calculate milliseconds since last update
    int msNow = glutGet(GLUT_ELAPSED_TIME);
    if (msLast == 0)
    {
        // bootstrap the first call
        msLast = msFps = msNow;
        return;
    }
    int elapsed = msNow - msLast;

    // update all modules
    io::update(elapsed);
    gfx::update(elapsed);
    global::stateManager->currentState->update(elapsed);
    global::networkManager->network->update(elapsed);

    InGameState *curstate = global::stateManager->currentState;
    for(int i = 0; i < (int)curstate->objects.size(); i++)
    {
      curstate->objects[i]->update(elapsed);
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
}

void initialize()
{
  
  global::width = 1024;
  global::height = 768;
  global::fullscreen = false;
  global::app_title = (char*)malloc(sizeof(char) * 80);
  sprintf(global::app_title, "2Cities : The Game");
  
  gl::init();
  
  // initialization of all other modules
  io::init();
  gfx::init();
  
  initState();

  global::networkManager->initialize();

  glutIdleFunc(updateLoop);
  
  glutMainLoop();
}

/*
 * application kickstart.
 */
int main(int argc, char** argv)
{

  if(argc == 2)
  {
    if(!strcmp(argv[1], "-build"))
    {
      beginState=BUILD_STATE;
    }
  }else{
    beginState=CARNAGE_STATE;
  }
  glutInit(&argc, argv);
  initialize();
  return 0;
}
