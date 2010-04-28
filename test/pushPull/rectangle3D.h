#ifndef OBJECTS_H
#define OBJECTS_H

#include "includes.h"

using namespace std;

/* enum for rectangle faces:
           
          +z 
           |
      _____|____
      |    2    |
 +x___|1       3|___-x
      |         |
      |____4____|
           |
           |
          -z

*/
enum Face{NONE = -1, TOP = 0, FACE1 = 1, FACE2 = 2, FACE3 = 3, FACE4 = 4, BOTTOM = 5};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Rectangle3D
 */
class Rectangle3D
{
public:
	int blocksize;
	intPoint min, max;
	Colour color;

	Rectangle3D();
   Rectangle3D(intPoint min, intPoint max, int blocksize);
	void draw_rectangle3D();
	void draw_rectangle2D();
	void draw();
	void adjust_bases();
	void print_rectangle();
   Face check_click(Point click);
   int compute_area();
   int compute_volume();
   bool encapsulates(intPoint max_testpt, intPoint min_testpt);
};

#endif
