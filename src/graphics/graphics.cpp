#include <GL/gl.h>
#include <GL/glut.h>

#include "graphics.h"

namespace gfx{

  Renderer renderer;
  Hud hud;

  void display()
  {
    renderer.draw();
    hud.draw();
    glutSwapBuffers();
  }
  
  void reshape(int w, int h)
  {
    renderer.reshape(w, h);
  }
  

  void init()
  {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
  }
  
  
  void update(long elapsed)
  {

    display();
  }
  
  
}
