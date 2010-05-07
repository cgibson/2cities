#include "quaternion.h"

Quaternion::Quaternion() {
        m_h = 1;
	m_i = m_j = m_k = 0;
}

Quaternion::Quaternion(float h, float i, float j, float k) {
	m_h = h;
	m_i = i;
	m_j = j;
	m_k = k;
}

Vector Quaternion::getAxis()
{
  return Vector(m_h, m_i, m_j);
}

float Quaternion::getAngle()
{
  return m_k;
}

void Quaternion::setAxis(Vector v)
{
  m_h = v.x();
  m_i = v.y();
  m_j = v.z();
}

void Quaternion::setAngle(float angle)
{
  m_k = angle;
}

float Quaternion::getH(){ return m_h; }
float Quaternion::getI(){ return m_i; }
float Quaternion::getJ(){ return m_j; }
float Quaternion::getK(){ return m_k; }
