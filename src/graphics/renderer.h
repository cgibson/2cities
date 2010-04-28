#include "../helper/Vector.h"

#ifndef _RENDERER_H_
#define _RENDERER_H_

class Renderer{
public:
  Renderer();
  void draw();
  void reshape(int w, int h);
private:
  void updateLookat();
  
  Vector eye, lookAt;
  
  float DEBUG_THETA;
};

#endif
