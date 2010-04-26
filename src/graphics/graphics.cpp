#include <GL/gl.h>
#include <GL/glut.h>

#include "renderer.h"

namespace gfx{

  Renderer renderer;
  
  void display()
  {
    renderer.draw();
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
