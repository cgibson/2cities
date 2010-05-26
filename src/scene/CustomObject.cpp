#include "CustomObject.h"
#include "../system/global.h"
#include "../graphics/graphics.h"
#include "stdio.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * CustomObject
 */

void CustomObject::update(int elapsedTime)
{
}
void CustomObject::think(int elapsedTime)
{
}

void CustomObject::set_max(Point newmax)
{
	set_max_x(newmax.getx());
	set_max_y(newmax.gety());
	set_max_z(newmax.getz());
}

void CustomObject::set_min(Point newmin)
{
	set_min_x(newmin.getx());
	set_min_y(newmin.gety());
	set_min_z(newmin.getz());
}

void CustomObject::print_rectangle()
{
	printf("---\nmin: %s\n", min.str());
	printf("max: %s\n", max.str());
}

// returns distance from point to plane (specified face)
// will adjust distance so if pt is "inside" the plane / face distance is negative
int CustomObject::distance2Face(Face f, Point pt)
{
	int d = 0;	
	if(f == FACE1)
	{
		d = pt.getx() - max.getx();
	}
	else if(f == FACE2)
	{
		d = pt.getz() - max.getz();
	}
	else if(f == FACE3)
	{
		d = min.getx() - pt.getx();
	}
	else if(f == FACE4)
	{
		d = min.getz() - pt.getz();
	}
	return d;
}

Point CustomObject::whichPoint(Face f)
{
	if(f == FACE1 || f == FACE2)
		return max;
	else if(f == FACE3 || f == FACE4)
		return min;
	else
		return min;
}

// returns true if the rectangle is inside our rect (excluding Face f)
bool CustomObject::semiEncapsulates(Face f, Point max_testpt, Point min_testpt)
{
   if(f == FACE1 && max_testpt.getz() <= max.getz() && min_testpt.getz() >= min.getz() && min_testpt.getx() >= min.getx() && min_testpt.getx() <= max.getx())
		return true;
	else if(f == FACE2 && max_testpt.getx() <= max.getx() && min_testpt.getx() >= min.getx() && min_testpt.getz() >= min.getz() && min_testpt.getz() <= max.getz())
		return true;
	else if(f == FACE3 && max_testpt.getz() <= max.getz() && min_testpt.getz() >= min.getz() && max_testpt.getx() <= max.getx() && max_testpt.getx() >= min.getx())
		return true;
	else if(f == FACE4 && max_testpt.getx() <= max.getx() && min_testpt.getx() >= min.getx() && max_testpt.getz() <= max.getz() && max_testpt.getz() >= min.getz())
		return true;
	return false;
}

// returns true if the parameters max and min are inside of our rectangle
bool CustomObject::encapsulates(Point max_testpt, Point min_testpt)
{
   // IF test_pt.x is inside our_rect.x 
   if(max_testpt.getx() <= max.getx() && min_testpt.getx() >= min.getx())
   {
      // IF test_pt.z is inside our_rect.z
      if(max_testpt.getz() <= max.getz() && min_testpt.getz() >= min.getz())
         return true;
   }
   return false;
}

// x * z
int CustomObject::compute_area()
{
   return (max.getx() - min.getx()) * (max.getz() - min.getz());
}

// x * y * z
int CustomObject::compute_volume()
{
   return (max.getx() - min.getx()) * (max.gety() - min.gety()) * (max.getz() - min.getz());
}

// this method takes a point and determines if that point intercets the rectangle
// it returns the face that it intercets or -1 if it does not intercet a face
Face CustomObject::check_click(Point click)
{
	//printf("checking click: %s\n", click.str());
	//printf("against rectangle: ");
	//print_rectangle();
	// IF the point is inside of a face, return that face
   Face intercetion = NOTHING;
   // check TOP - xz plane, max.gety()
   if(fabs(click.gety() - max.gety()) < 1)
   {
		//printf("checking top\n");      
		if(click.getz() > min.getz() && click.getz() < max.getz())
      {
         if(click.getx() > min.getx() && click.getx() < max.getx())
         {
            intercetion = TOP;
            return intercetion;
         }
      }
   }

   // check FACE1 - zy plane, max.getx()
   if(fabs(click.getx() - max.getx()) < 1)
   {
		//printf("checking FACE1\n");      
		if(click.getz() > min.getz() && click.getz() < max.getz())
      {
         //printf("within z bounds\n");
			if(click.gety() > min.gety() && click.gety() < max.gety())
         {
				//printf("within y bounds\n");
            intercetion = FACE1;
            return intercetion;
         }
      }
   }

   // check FACE2 - xy plane, max.getz()
   if(fabs(click.getz() - max.getz()) < 1)
   {
      if(click.getx() > min.getx() && click.getx() < max.getx())
      {
         if(click.gety() > min.gety() && click.gety() < max.gety())
         {
            intercetion = FACE2;
            return intercetion;
         }
      }
   }

   // check FACE3 - zy plane, min.getx()
   if(fabs(click.getx() - min.getx()) < 1)
   {
      if(click.getz() > min.getz() && click.getz() < max.getz())
      {
         if(click.gety() > min.gety() && click.gety() < max.gety())
         {
            intercetion = FACE3;
            return intercetion;
         }
      }
   }

   // check FACE4 - xy plane, min.getz()
   if(fabs(click.getz() - min.getz()) < 1)
   {
      if(click.getx() > min.getx() && click.getx() < max.getx())
      {
         if(click.gety() > min.gety() && click.gety() < max.gety())
         {
            intercetion = FACE4;
            return intercetion;
         }
      }
   }

   // check BOTTOM - xz plane, min.gety()
   if(fabs(click.gety() - min.gety()) < 1)
   {
      if(click.getx() > min.getx() && click.getx() < max.getx())
      {
         if(click.getz() > min.getz() && click.getz() < max.getz())
         {
            intercetion = BOTTOM;
            return intercetion;
         }
      }
   }

   return intercetion;
}

void CustomObject::adjust_bases()
{
	if(min.getz() > max.getz())
	{
		Point temp = max;
		max = min;
		min = temp;
	}
	
	if(max.getx() < min.getx())
	{
		Point temp = max;
		max.setx(min.getx());
		min.setx(temp.getx());
	}
}

// p1 and p2 are two points on the ground that represent a 2D rectangle
void CustomObject::orientRect(Point p1, Point p2)
{
	// set x	
	max.setx(p1.getx());
	min.setx(p2.getx());
	if(p2.getx() > max.getx())
	{
		max.setx(p2.getx());
		min.setx(p1.getx());
	}
	
	// set z
	max.setz(p1.getz());
	min.setz(p2.getz());
	if(p2.getz() > max.getz())
	{
		max.setz(p2.getz());
		min.setz(p1.getz());
	}
}

void CustomObject::draw_rectangle2D()
{		
	bool higherRect = false;
	double plus = 0;
	//printf("print 2D min.gety() = %d\n", min.gety());
   if(min.gety() <= 0)
	{
		higherRect = true;
		plus += 0.1;
	}

	glBegin(GL_POLYGON);
	//glColor3f(color.red, color.green, color.blue);
	glVertex3f(min.getx(), plus + min.gety(), min.getz());
	glVertex3f(max.getx(), min.gety() + plus, min.getz());
	glVertex3f(max.getx(), max.gety() + plus, max.getz());
	glVertex3f(min.getx(), max.gety() + plus, max.getz());
	glEnd();
}

void CustomObject::draw_rectangle3D()
{
	// draw TOP
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	glVertex3f(max.getx(), max.gety(), max.getz());
	glVertex3f(min.getx(), max.gety(), max.getz());
	glVertex3f(min.getx(), max.gety(), min.getz());
	glVertex3f(max.getx(), max.gety(), min.getz());
	glEnd();	

	// draw BOTTOM
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	glVertex3f(max.getx(), min.gety(), max.getz());
	glVertex3f(min.getx(), min.gety(), max.getz());
	glVertex3f(min.getx(), min.gety(), min.getz());
	glVertex3f(max.getx(), min.gety(), min.getz());
	glEnd();	

	// draw side 1
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);	
	glVertex3f(max.getx(), min.gety(), max.getz());
	glVertex3f(max.getx(), max.gety(), max.getz());
	glVertex3f(max.getx(), max.gety(), min.getz());
	glVertex3f(max.getx(), min.gety(), min.getz());
	glEnd();	

	// draw side 2
	glBegin(GL_POLYGON);
	glNormal3f(0,0,1);
	glVertex3f(min.getx(), min.gety(), max.getz());
	glVertex3f(min.getx(), max.gety(), max.getz());
	glVertex3f(max.getx(), max.gety(), max.getz());
	glVertex3f(max.getx(), min.gety(), max.getz());
	glEnd();	

	// draw side 3
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);
	glVertex3f(min.getx(), min.gety(), min.getz());
	glVertex3f(min.getx(), max.gety(), min.getz());
	glVertex3f(min.getx(), max.gety(), max.getz());
	glVertex3f(min.getx(), min.gety(), max.getz());
	glEnd();	
	
	// draw side 4
	glBegin(GL_POLYGON);
	glNormal3f(0,0,1);
	glVertex3f(max.getx(), min.gety(), min.getz());
	glVertex3f(max.getx(), max.gety(), min.getz());
	glVertex3f(min.getx(), max.gety(), min.getz());
	glVertex3f(min.getx(), min.gety(), min.getz());
	glEnd();	

}

void CustomObject::draw() {

/*
   Blueprint blueprint = global::factory->getBlueprint(type);

	gfx::materials[blueprint.getMaterial()].applyMaterial(gfx::cur_shader, "frontMat");
*/
	// IF height = 0, THEN draw square at y = 0
	if(min.gety() == max.gety())
		draw_rectangle2D();
	// ELSE, THEN draw rectangle
	else
		draw_rectangle3D();
}
