#ifndef _RENDERER_H_
#define _RENDERER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <GL/glew.h>
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helper/Vector.h"
#include "../system/global.h"
#include "../helper/FBOHelper.h"
#include "Material.h"
#include "Lighting.h"
#include "../state/CarnageState.h"

using namespace std;

class Renderer{
public:
  Renderer();
  void init();
  void draw();
  void draw_scene();
  void reshape(int w, int h);
private:
  void update_view();
  void init_lights();
  void do_lights();
  void updateLookat();
  void init_materials();
  
  bool initFBO();
  
  Vector eye, lookAt;
  
  Lighting light;
  Material groundMat;
  Material blockMat;
  Material ammoMat;
};

#endif
