#include "includes.h"
#include "rectangle3D.h"
#include <iostream>
#include <fstream>

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Rectangle3D
 */

// Rectangle3D constructor
Rectangle3D::Rectangle3D(intPoint mn, intPoint mx, int blockSize)
{
	blocksize = blockSize;	
	min = mn;
	max = mx;
}

// Rectangle3D default constructor
Rectangle3D::Rectangle3D()
{
	blocksize = 1;   
	min = intPoint();
   max = intPoint();
}

void Rectangle3D::print_rectangle()
{
	printf("---\nmin:\n");
	print_ipoint(min);
	printf("max:\n");
	print_ipoint(max);
}

// returns distance from point to plane (specified face)
// will adjust distance so if pt is "inside" the plane / face distance is negative
int Rectangle3D::distance2Face(Face f, Point pt)
{
	int d = 0;	
	if(f == FACE1)
	{
		d = pt.x - max.x;
	}
	else if(f == FACE2)
	{
		d = pt.z - max.z;
	}
	else if(f == FACE3)
	{
		d = min.x - pt.x;
	}
	else if(f == FACE4)
	{
		d = min.z - pt.z;
	}
	return d;
}

intPoint Rectangle3D::whichPoint(Face f)
{
	if(f == FACE1 || f == FACE2)
		return max;
	else if(f == FACE3 || f == FACE4)
		return min;
}

// returns true if the rectangle is inside our rect (excluding Face f)
bool Rectangle3D::semiEncapsulates(Face f, intPoint max_testpt, intPoint min_testpt)
{
   if(f == FACE1 && max_testpt.z <= max.z && min_testpt.z >= min.z && min_testpt.x >= min.x && min_testpt.x <= max.x)
		return true;
	else if(f == FACE2 && max_testpt.x <= max.x && min_testpt.x >= min.x && min_testpt.z >= min.z && min_testpt.z <= max.z)
		return true;
	else if(f == FACE3 && max_testpt.z <= max.z && min_testpt.z >= min.z && max_testpt.x <= max.x && max_testpt.x >= min.x)
		return true;
	else if(f == FACE4 && max_testpt.x <= max.x && min_testpt.x >= min.x && max_testpt.z <= max.z && max_testpt.z >= min.z)
		return true;
	return false;
}

// returns true if the parameters max and min are inside of our rectangle
bool Rectangle3D::encapsulates(intPoint max_testpt, intPoint min_testpt)
{
   // IF test_pt.x is inside our_rect.x 
   if(max_testpt.x <= max.x && min_testpt.x >= min.x)
   {
      // IF test_pt.z is inside our_rect.z
      if(max_testpt.z <= max.z && min_testpt.z >= min.z)
         return true;
   }
   return false;
}

// x * z
int Rectangle3D::compute_area()
{
   return (max.x - min.x) * (max.z - min.z);
}

// x * y * z
int Rectangle3D::compute_volume()
{
   return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

// this method takes a point and determines if that point intercets the rectangle
// it returns the face that it intercets or -1 if it does not intercet a face
Face Rectangle3D::check_click(Point click)
{
   // IF the point is inside of a face, return that face
   Face intercetion = NONE;
   // check TOP - xz plane, max.y
   if(fabs(click.y - max.y) < 1)
   {
      if(click.z > min.z && click.z < max.z)
      {
         if(click.x > min.x && click.x < max.x)
         {
            intercetion = TOP;
            return intercetion;
         }
      }
   }

   // check FACE1 - zy plane, max.x
   if(fabs(click.x - max.x) < 1)
   {
      if(click.z > min.z && click.z < max.z)
      {
         if(click.y > min.y && click.y < max.y)
         {
            intercetion = FACE1;
            return intercetion;
         }
      }
   }

   // check FACE2 - xy plane, max.z
   if(fabs(click.z - max.z) < 1)
   {
      if(click.x > min.x && click.x < max.x)
      {
         if(click.y > min.y && click.y < max.y)
         {
            intercetion = FACE2;
            return intercetion;
         }
      }
   }

   // check FACE3 - zy plane, min.x
   if(fabs(click.x - min.x) < 1)
   {
      if(click.z > min.z && click.z < max.z)
      {
         if(click.y > min.y && click.y < max.y)
         {
            intercetion = FACE3;
            return intercetion;
         }
      }
   }

   // check FACE4 - xy plane, min.z
   if(fabs(click.z - min.z) < 1)
   {
      if(click.x > min.x && click.x < max.x)
      {
         if(click.y > min.y && click.y < max.y)
         {
            intercetion = FACE4;
            return intercetion;
         }
      }
   }

   // check BOTTOM - xz plane, min.y
   if(fabs(click.y - min.y) < 1)
   {
      if(click.x > min.x && click.x < max.x)
      {
         if(click.z > min.z && click.z < max.z)
         {
            intercetion = BOTTOM;
            return intercetion;
         }
      }
   }

   return intercetion;
}

void Rectangle3D::draw_rectangle2D()
{		
	bool higherRect = false;
	int plus = 0;
	//printf("print 2D min.y = %d\n", min.y);
   if(min.y > 0)
	{
		higherRect = true;
		plus += 1;
	}

	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(min.x, min.y + plus, min.z);
	glVertex3f(max.x, min.y + plus, min.z);
	glVertex3f(max.x, max.y + plus, max.z);
	glVertex3f(min.x, max.y + plus, max.z);
	glEnd();
}

void Rectangle3D::draw_rectangle3D()
{
	// draw TOP
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glEnd();	

	// draw BOTTOM
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glEnd();	

	// draw side 1
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glEnd();	

	// draw side 2
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, min.y, max.z);
	glEnd();	

	// draw side 3
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glEnd();	
	
	// draw side 4
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, min.y, min.z);
	glEnd();	

}

void Rectangle3D::draw()
{
	// IF height = 0, THEN draw square at y = 0
	if(min.y == max.y)
		draw_rectangle2D();
	// ELSE, THEN draw rectangle
	else
		draw_rectangle3D();
}

void Rectangle3D::adjust_bases()
{
	if(min.z > max.z)
	{
		intPoint temp = max;
		max = min;
		min = temp;
	}
	
	if(max.x < min.x)
	{
		intPoint temp = max;
		max.x = min.x;
		min.x = temp.x;
	}
}

