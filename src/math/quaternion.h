#ifndef _QUATERNION_H_
#define _QUATERNION_H_

class Quaternion {
public:
  Quaternion();
  Quaternion(float h, float i, float j, float k);
private:
  float m_h, m_i, m_j, m_k;
};

#endif