#ifndef _VECTOR_H_
#define _VECTOR_H_

#define FLOAT_EPSILON 0.0001f

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Vector {
public:
  Vector();
  Vector( const Vector& v );
  Vector(double x, double y, double z);
  double x();
  double y();
  double z();
  double *array();
  
  void x(double x){p_x = x;}
  void y(double y){p_y = y;}
  void z(double z){p_z = z;}
  
  Vector cross( Vector v );
  
  char *str();
  
  double distance( Vector v );
  
  double norm();
  double mag();

  Vector operator=( const Vector& );

  Vector operator+( const Vector& v )
    { return Vector( p_x + v.p_x, p_y + v.p_y, p_z + v.p_z); }
    
  Vector operator-( const Vector& v )
    { return Vector( p_x - v.p_x, p_y - v.p_y, p_z - v.p_z); }
     
  double operator*( const Vector& v )
    { return ( p_x*v.p_x + p_y*v.p_y + p_z*v.p_z); }
	
  Vector operator*(const double f) 
    { return Vector( p_x*f , p_y*f , p_z*f); }
	
  Vector operator/(const double f) 
    { return Vector( p_x/f , p_y/f , p_z/f); }
    
  Vector mult( const Vector& v )
  {
     return Vector( p_x * v.p_x, p_y * v.p_y, p_z * v.p_z);
  }
  
  bool equals( const Vector& v )
  {
    return ((p_x < (v.p_x + FLOAT_EPSILON)) &&
            (p_x > (v.p_x - FLOAT_EPSILON)) &&
            (p_y < (v.p_x + FLOAT_EPSILON)) &&
            (p_y > (v.p_x - FLOAT_EPSILON)) &&
            (p_z < (v.p_x + FLOAT_EPSILON)) &&
            (p_z > (v.p_x - FLOAT_EPSILON)));
  }
    
//  friend ostream& operator<<( ostream&, const Vector& );
//  friend istream& operator>>( istream&, Vector& );

private:
  double p_x, p_y, p_z;
};

#endif
