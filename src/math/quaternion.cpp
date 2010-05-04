#include "quaternion.h"

Quaternion::Quaternion() {
	m_h = m_i = m_j = m_k = 0;
}

Quaternion::Quaternion(float h, float i, float j, float k) {
	m_h = h;
	m_i = i;
	m_j = j;
	m_k = k;
}
