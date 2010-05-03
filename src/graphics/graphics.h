#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <string>

#include "Lighting.h"
#include "renderer.h"
#include "hud.h"
#include "../system/io.h"
#include "../helper/GLSL_helper.h"

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
