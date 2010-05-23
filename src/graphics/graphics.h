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
#include "modelhandler.h"
#include "hud.h"
#include "../system/io.h"
#include "../helper/GLSL_helper.h"
#include "../system/enum.h"

using namespace enumeration;

namespace gfx{

  extern Renderer renderer;
  extern ModelHandler modelHandler;
  extern Hud hud;
    
  extern Material* materials;
  
  extern GLint shSimple;          // simple shader
  extern GLint shBuildGrid;
  extern GLint shForceBlock;
  
  extern bool draw_wireframe;
  extern bool draw_axis;
  
  extern GLint cur_shader;
  
  void useShader(GLint program);
  void display();
  void reshape(int w, int h);
  void init();
  void update(int elapsed);
  
  bool installShader( string filename, int *shaderID );
  bool loadShaders();

}

#endif
