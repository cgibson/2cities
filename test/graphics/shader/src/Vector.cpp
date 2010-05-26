#include "include/Vector.h"
#include "stdio.h"
#include "math.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

Vector::Vector(GLdouble x, GLdouble y, GLdouble z)
{
  p_x = x;
  p_y = y;
  p_z = z;
}

Vector Vector::operator=( const Vector& v1) {
	this->p_x = v1.p_x;
	this->p_y = v1.p_y;
	this->p_z = v1.p_z;
	return(*this );
}

Vector::Vector( const Vector& v )
{
  p_x = v.p_x;
  p_y = v.p_y;
  p_z = v.p_z;
}

Vector::Vector()
{
  p_x = 0;
  p_y = 0;
  p_z = 0;
}

double Vector::distance( Vector v )
{
  Vector diff = Vector( p_x - v.p_x, p_y - v.p_y, p_z - v.p_z );
  return diff.norm();
}

double Vector::norm()
{
  double d = sqrt((p_x * p_x) + (p_y * p_y) + (p_z * p_z));
  p_x /= d;
  p_y /= d;
  p_z /= d;
  return d;
}

double Vector::mag()
{
  return sqrt((p_x * p_x) + (p_y * p_y) + (p_z * p_z));
}

char* Vector::str( void ) {
    char *buffer = (char*)calloc(300, sizeof(char));
    sprintf(buffer, "<%.2f, %.2f, %.2f>", p_x, p_y, p_z);
    return buffer;
}

GLdouble Vector::x()
{
  return p_x;
}

GLdouble Vector::y()
{
  return p_y;
}

GLdouble Vector::z()
{
  return p_z;
}

GLdouble *Vector::array()
{
  GLdouble *ret = (GLdouble*)malloc(sizeof(GLdouble) * 3);
  ret[0] = p_x;
  ret[1] = p_y;
  ret[2] = p_z;
  return ret;
}
