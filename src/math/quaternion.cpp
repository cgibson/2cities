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

char* Quaternion::str() {
	char *buffer = new char[200];
	sprintf(buffer, "<%.2f, %.2f, %.2f, %.2f>", m_h, m_i, m_j, m_k);
	return buffer;
}

Quaternion Quaternion::GenerateRotationFromDirectionVector(Vector vDirection)
        {
            // Step 1. Setup basis vectors describing the rotation given the input vector and assuming an initial up direction of (0, 1, 0)
            Vector vUp = Vector(0, 1.0f, 0.0f);           // Y Up vector
            Vector vRight = vUp.cross(vDirection);    // The perpendicular vector to Up and Direction
            vUp = vDirection.cross(vRight);            // The actual up vector given the direction and the right vector
            
	    //printf("up %s\n", vUp.str());
	    //printf("right %s\n", vRight.str());
	    //printf("dir %s\n", vDirection.str());
            // Step 2. Put the three vectors into the matrix to bulid a basis rotation matrix
            // This step isnt necessary, but im adding it because often you would want to convert from matricies to quaternions instead of vectors to quaternions
            // If you want to skip this step, you can use the vector values directly in the quaternion setup below
            //Matrix mBasis = new Matrix(vRight.x(),    vRight.y(),   vRight.Z, 0.0f,
            //                            vUp.X,        vUp.Y,        vUp.Z, 0.0f,
            //                            vDirection.X, vDirection.Y, vDirection.Z, 0.0f,
            //                            0.0f, 0.0f, 0.0f, 1.0f);
            
            // Step 3. Build a quaternion from the matrix
	    
            float k = (float)sqrt(1.0f + vRight.x() + vUp.y() + vDirection.z()) / 2.0f;
            double dfWScale = k * 4.0;
	    //printf("dfWScale: %f\n", dfWScale);
            float h = (float)((vUp.z() - vDirection.y()) / dfWScale); //32, 23
            float i = (float)((vDirection.x() - vRight.z()) / dfWScale); //13, 31
            float j = (float)((vRight.y() - vUp.x()) / dfWScale); //21, 12
            //float h = (float)((vDirection.y() - vUp.z()) / dfWScale); //32, 23
            //float i = (float)((vRight.z() - vDirection.x()) / dfWScale); //13, 31
            //float j = (float)((vUp.x() - vRight.y()) / dfWScale); //21, 12
	    
	    float scale = sqrt(h * h + i * i + j * j);
	    float ax = h / scale;
	    float ay = i / scale;
	    float az = j / scale;
	    float angle = acos(k) * 2.0f;

            return Quaternion(ax, ay, az, angle * 180.0 / 3.14);
        }
