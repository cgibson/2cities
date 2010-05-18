#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "../helper/Vector.h"

class Quaternion {
public:
  Quaternion();
  Quaternion(float h, float i, float j, float k);
  
  Vector getAxis();
  void setAxis(Vector axis);
  float getAngle();
  void setAngle(float angle);
  float getH();
  float getI();
  float getJ();
  float getK();

  char *str();
private:
  float m_h, m_i, m_j, m_k;
};

#endif
