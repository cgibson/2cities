#include <GL/gl.h>
#include <GL/glut.h>
#include <string>

#include "renderer.h"
#include "hud.h"

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

namespace gfx{

  extern Renderer renderer;
  extern Hud hud;
  
  
  extern GLint shSimple;          // simple shader
  
  extern bool draw_wireframe;

  void display();
  void reshape(int w, int h);
  void init();
  void update(int elapsed);
  
  bool installShader( string filename, int *shaderID );
  bool loadShaders();

}

#endif
