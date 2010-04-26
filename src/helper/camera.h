#include "Vector.h"

#ifndef _CAMERA_H_
#define _CAMERA_H_

class Camera{
public:
  Vector eye;
  Vector lookAt;
  float fov;
  float near;
  float far;
};

#endif
