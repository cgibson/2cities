#ifndef _RENDERER_H_
#define _RENDERER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helper/Vector.h"
#include "../system/global.h"
#include "camera.h"
#include "../helper/FBOHelper.h"
#include "Material.h"
#include "Lighting.h"
#include "../state/CarnageState.h"
#include "skybox.h"
#include "shader.h"

using namespace std;

class Renderer{
public:
  Renderer();
  void init();
  void update(int elapsed);
  void draw();
  void drawDiffusePass();
  void drawBloomPass();
  void draw_scene();
  void reshape(int w, int h);
  void getMatrices(GLdouble **mvMatrix, GLdouble **pMatrix, GLint **vMatrix);

private:
  void update_view();
  void init_lights();
  void do_lights(Shader sh);
  bool initFBO();

  Vector eye, lookAt;

  Lighting light;

  Skybox skybox;
  
  FBOHelper fbo;
};

#endif
