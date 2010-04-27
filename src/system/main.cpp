#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "global.h"
#include "gl_helper.h"
#include "../graphics/graphics.h"

long lastmsec = 0;
bool running;

void timer( int step )
{
  struct timeb tp;
  ftime(&tp);

  long currmsec = tp.time * 1000 + tp.millitm;
  //printf("LAST: %d\n", curr
  if(lastmsec == 0)
    lastmsec = currmsec;
    
  long elapsed = currmsec - lastmsec;
    
  // update all modules
  gfx::update(elapsed);

  glutTimerFunc(1, timer, 0);
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

  glutTimerFunc(1, timer, 0);
  
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
