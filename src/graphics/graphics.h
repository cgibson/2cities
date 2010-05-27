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
#include "shader.h"

using namespace enumeration;

namespace gfx{
  extern bool WorldObjectPtr_Sort_Type(WorldObject * objPtr1, WorldObject * objPtr2);
  extern int ObjTypeSortIndex;

  extern Renderer renderer;
  extern ModelHandler modelHandler;
  extern Hud hud;

  extern Material* materials;

  extern DefaultShader simpleShader;
  extern DefaultShader gridShader;
  extern DefaultShader forceBlockShader;
  extern DefaultShader skyscraperShader;
  extern DefaultShader skyShader;
  extern DefaultShader distantShader;

  extern bool draw_wireframe;
  extern bool draw_axis;

  void loadDefaultShaderValues();
  void display();
  void reshape(int w, int h);
  void init();
  void update(int elapsed);

  bool installShader( string filename, int *shaderID );
  bool loadShaders();

}

#endif
