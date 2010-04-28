#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "gl_helper.h"
#include "../graphics/graphics.h"

int msLast = 0;
bool running;

void updateLoop()
{
    // calculate milliseconds since last update
    int msNow = glutGet(GLUT_ELAPSED_TIME);
    if (msLast == 0)
    {
        // bootstrap the first call
        msLast = msNow;
        return;
    }
    int elapsed = msNow - msLast;

    // update all modules
    gfx::update(elapsed);
}

bool initialize()
{
  
  global::width = 800;
  global::height = 600;
  global::fullscreen = false;
  global::app_title = (char*)malloc(sizeof(char) * 80);
  sprintf(global::app_title, "2Cities : The Game");
  
  gl::init();
  
  // initialization of all other modules
  gfx::init();

  glutIdleFunc(updateLoop);
  
  glutMainLoop();
}

/*
 * application kickstart.
 */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  bool r = initialize();
  return 0;
}
