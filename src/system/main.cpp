#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "gl_helper.h"
#include "../graphics/graphics.h"
#include "../state/CarnageState.h"

int msLast = 0;
bool running;

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
    gfx::update(elapsed);
    global::stateManager.currentState->update(elapsed);
    
    ((CarnageState*)global::stateManager.currentState)->physics.update(elapsed);

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
  global::stateManager.changeCurrentState(CARNAGE_S);
  CarnageState *state = (CarnageState*)global::stateManager.currentState;
}

void initialize()
{
  
  global::width = 800;
  global::height = 600;
  global::fullscreen = false;
  global::app_title = (char*)malloc(sizeof(char) * 80);
  sprintf(global::app_title, "2Cities : The Game");
  
  gl::init();
  
  // initialization of all other modules
  gfx::init();
  
  initState();

  glutIdleFunc(updateLoop);
  
  glutMainLoop();
}

/*
 * application kickstart.
 */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  initialize();
  return 0;
}
