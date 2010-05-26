#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Vector {
public:
  Vector();
  Vector( const Vector& v );
  Vector(GLdouble x, GLdouble y, GLdouble z);
  GLdouble x();
  GLdouble y();
  GLdouble z();
  GLdouble *array();
  
  void x(GLdouble x){p_x = x;}
  void y(GLdouble y){p_y = y;}
  void z(GLdouble z){p_z = z;}
  
  char *str();
  
  double distance( Vector v );
  
  double norm();
  double mag();

  Vector operator=( const Vector& );

  Vector operator+( const Vector& v )
    { return Vector( p_x + v.p_x, p_y + v.p_y, p_z + v.p_z); }
    
  Vector operator-( const Vector& v )
    { return Vector( p_x - v.p_x, p_y - v.p_y, p_z - v.p_z); }
     
  GLdouble operator*( const Vector& v )
    { return ( p_x*v.p_x + p_y*v.p_y + p_z*v.p_z); }
	
  Vector operator*(const GLdouble f) 
    { return Vector( p_x*f , p_y*f , p_z*f); }
	
  Vector operator/(const GLdouble f) 
    { return Vector( p_x/f , p_y/f , p_z/f); }
    
  Vector mult( const Vector& v )
  {
     return Vector( p_x * v.p_x, p_y * v.p_y, p_z * v.p_z);
  }
    
//  friend ostream& operator<<( ostream&, const Vector& );
//  friend istream& operator>>( istream&, Vector& );

private:
  GLdouble p_x, p_y, p_z;
};

#endif
