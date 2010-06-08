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
#include "../helper/queue.h"

using namespace std;

class Renderer{
public:
  Renderer();
  void init();
  void update(int elapsed);
  void draw();
  void draw_screen();
  void draw_diffusePass();
  void drawBloomPass();
  void draw_scene();
  void reshape(int w, int h);
  void getMatrices(GLdouble **mvMatrix, GLdouble **pMatrix, GLint **vMatrix);

  Skybox skybox;
  Lighting light;

private:
  void update_view();
  void init_lights();
  void do_lights(Shader sh);
  void updateBloom(int elapsed);
  Vector eye, lookAt;
  float hackyRotate;

  bool use_fbo;

  LimitedQueue *fpsQueue;

  static const int min_test;
  static const int max_test;
  int test_count;
};

#endif
