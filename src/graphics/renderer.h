#include "../helper/Vector.h"
#include "../helper/FBOHelper.h"
#include "Material.h"
#include "Lighting.h"

using namespace std;

#ifndef _RENDERER_H_
#define _RENDERER_H_

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
};

#endif
