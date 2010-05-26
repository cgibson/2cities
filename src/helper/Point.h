#ifndef _POINT_H_
#define _POINT_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../graphics/graphics.h"

class Point {
private:
  GLdouble p_x, p_y, p_z;
public:
  Point();
  Point( const Point& p );
  Point(GLdouble clicked_x, GLdouble clicked_y);
  Point(GLdouble x, GLdouble y, GLdouble z);
  GLdouble getx();
  GLdouble gety();
  GLdouble getz();
  
  void setx(GLdouble x){p_x = x;}
  void sety(GLdouble y){p_y = y;}
  void setz(GLdouble z){p_z = z;}
	  
  void set(Point p){p_x = p.p_x;p_y = p.p_y;p_z = p.p_z;}
  void round();
  double roundDouble(double p);
	
	char *str();
  
	void adjustPointForBlocksize(int blocksize);

	Point operator=( const Point& );

//  friend ostream& operator<<( ostream&, const Vector& );
//  friend istream& operator>>( istream&, Vector& );
};

#endif
