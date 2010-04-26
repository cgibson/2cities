#include <GL/gl.h>
#include <GL/glut.h>

#include "renderer.h"

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

namespace gfx{

  extern Renderer renderer;

  void display();
  void reshape(int w, int h);
  void init();
  void update(long elapsed);

}

#endif
