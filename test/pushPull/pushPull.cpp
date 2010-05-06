/*
 * rect_element - test program demonstrating Google ScketchUp's Push and Pull tool
 */

#include "includes.h"
#include "rectangle3D.h"
#include <iostream>
#include <fstream>

#define DEPTH -20.f

using namespace std;

//other globals
int GW;
int GH;
Point last, mouse_pos = Point(0,0, 0);
GLfloat light_pos[4] = {0.0, 100.0, 0.0, 1.0};
int city_radius = 150;
int blocksize = 10;

// boolean variables
int draw_axis = 1, rect_element = -1;
bool delete_mode = false, look_around = false, mouse_move = false, strafe = false, pauseGame = false, circles = true;
Face intersection = NONE;
Point mouse_click;

// camera variables
Vector zoom_vector, up_vector, strafe_vector;
Point lookAt_pt, eye_pt;
double theta, phi, radius;

// vector of mouse clicks
std::vector<Point> points;
std::vector<Rectangle3D*> rectangles;
intPoint firstPoint = intPoint(-1, -1, -1);
int counter = 0;

//forward declarations of functions
void keyboard (unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMovement(int x, int y);
void display();
void push_pull(Point mouse_pos);
void new_push_pull(Point mouse_pos);
void draw_pushPullPlanes();
void print_rectangles();
void recursive_push(Face f, int bottom);
void adjust_face(int index, Face f, Point mouse_pos, bool block, bool pull);

/* file format
min.x min.y min.z max.x max.y max.z
min.x min.y min.z max.x max.y max.z
min.x min.y min.z max.x max.y max.z
....
*/
void saveRects2File(string filename)
{
   ofstream myfile;
   myfile.open(filename.c_str());
	myfile << "blocksize: " << blocksize << endl;
   for(int i = 0; i < rectangles.size(); i++)
   {
      // write rectangle's max and min to file
      myfile << (rectangles[i]->min.x) << " " << (rectangles[i]->min.y) << " " << (rectangles[i]->min.z) << " " 
             << (rectangles[i]->max.x) << " " << (rectangles[i]->max.y) << " " << (rectangles[i]->max.z) << endl;
   }
}

void print_camera_variables()
{
	printf("-------\ntheta %f  phi %f\n*eye_pt*\n", theta, phi);
	print_point(eye_pt);
	printf("*lookAt_pt*\n");
	print_point(lookAt_pt);
	printf("*zoom_vector (%f)\n", distance_between(zoom_vector.tail, zoom_vector.head));
	print_vector(zoom_vector);
	printf("*up_vector(%f)\n", distance_between(up_vector.tail, up_vector.head));
	print_vector(up_vector);
	printf("*strafe_vector(%f)\n", distance_between(strafe_vector.tail, strafe_vector.head));
	print_vector(strafe_vector);
	printf("-------\n");
}

void reshape(int w, int h)
{
   GW = w;
   GH = h;
  
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
	gluPerspective(60, GW / (float)GH, 1.0, 500);
	glMatrixMode(GL_MODELVIEW);
   glViewport(0, 0, w, h);
}

void enable()
{
   glEnable(GL_COLOR_MATERIAL);
   glEnable (GL_DEPTH_TEST); //enable the depth testing
   glEnable (GL_LIGHTING); //enable the lighting
   glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
   glShadeModel (GL_SMOOTH); //set the shader to smooth shader
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
}

void pos_light()
{
   //set the light's position
   glMatrixMode(GL_MODELVIEW);
   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

// adjusts tail and head values for zoom, up, and stafe vectors
// after a change in the eye / lookAt positions
void update_vectors()
{
	zoom_vector = polar_to_vector(eye_pt, theta, phi, 1.0);
	zoom_vector = normalize_vector(zoom_vector);

	// add 90 deg to theta to get up vector
	//up_vector = polar_to_vector(eye_pt, theta, phi - 270, 1.0);
	up_vector = polar_to_vector(eye_pt, theta, phi - 90, 1.0);
	up_vector = normalize_vector(up_vector);
	
	// strafe vector is the cross of zoom and up
	strafe_vector = crossProduct(zoom_vector, up_vector);
	strafe_vector = normalize_vector(strafe_vector);
}

// accepts as arguments the sign of the movement ( w vs s / a vs d )
// and the vector you're moving along ( either +/- strafe or +/- zoom )
void wads(int sign, Vector move_along)
{
	eye_pt.x += sign * (move_along.head.x - move_along.tail.x);
	eye_pt.y += sign * (move_along.head.y - move_along.tail.y);
	eye_pt.z += sign * (move_along.head.z - move_along.tail.z);
	lookAt_pt.x += sign * (move_along.head.x - move_along.tail.x);
	lookAt_pt.y += sign * (move_along.head.y - move_along.tail.y);
	lookAt_pt.z += sign * (move_along.head.z - move_along.tail.z);
}

void keyboard (unsigned char key, int x, int y)
{
   char filename[20];
   switch(key)
   {
		// move character forward
      case 's':
         wads(-1 * 5, zoom_vector);
         break;

      // move character backward
      case 'w':
			wads(1 * 5, zoom_vector);
         break;
      
      case 'a':
         wads(-1 * 5, strafe_vector);
         break;
      
      case 'd':
         wads(1 * 5, strafe_vector);
         break;
		
		// toggle axis and grid
      case 'm':
         draw_axis *= -1;
         break;

      // toggle delete mode
      case ' ':
         if(delete_mode)
            printf("DELETE_MODE OFF\n");
         else
            printf("DELETE_MODE ON\n");
         delete_mode = !delete_mode;
         break;

      // save rectangles vector to file: "rectangles.txt"
      case 'v':
         printf("SAVED RECTANGLES TO FILE: ");
         scanf("%s", filename);
         saveRects2File(filename);
         break;

		// exit
      case 27:
         exit(0);
   }
   glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_MIDDLE_BUTTON)
   {
      if(state == GLUT_DOWN)
		{
			last.x = x;
			last.y = y;         
			look_around = true;
			mouse_move = false;
		}      
		else if(state == GLUT_UP)
      {
         look_around = false;
         mouse_move = false;
      }
   }   
	else if (button == GLUT_RIGHT_BUTTON && !delete_mode)
   {
		if(state == GLUT_DOWN)
      {
         firstPoint.x = x;
			firstPoint.y = y;
      }
      else if(state == GLUT_UP)
      {
			Point click = click2point(x, y);
			// adjust click for blocksize
			if(click.z < 0)
				click.z = click.z - ((int)click.z % -blocksize);
			else
				click.z = click.z - ((int)click.z % blocksize);			
			if(click.x < 0)
				click.x = click.x - ((int)click.x % -blocksize);
			else
				click.x = click.x - ((int)click.x % blocksize);			
			
			rectangles[rectangles.size() - 1]->min = firstPoint;
			rectangles[rectangles.size() - 1]->max.x = click.x;
			rectangles[rectangles.size() - 1]->max.z = click.z;
			rectangles[rectangles.size() - 1]->max.y = rectangles[rectangles.size() - 1]->min.y;
			rectangles[rectangles.size() - 1]->adjust_bases();

			firstPoint.x = -1;
			firstPoint.y = -1;
			counter = 0;

			if(rectangles.size() != 0)
				rectangles[rectangles.size() - 1]->print_rectangle();
      }
   }
   else if (button == GLUT_LEFT_BUTTON)
   {
		if(state == GLUT_DOWN)
		{
			Point click = click2point(x, y);
         Face temp_face;
			// IF point is inside of a rectangle
			for(int i = 0; i < rectangles.size(); i++)
			{
            // always take the rectangle with the higher min(it's the TOP one)
            temp_face = rectangles[i]->check_click(click);
            if(temp_face != NONE)
            {
					// IF there isn't a different potential face
					if(intersection == NONE)
               {
                  intersection = temp_face;
                  rect_element = i;
               }   
               // IF old face has a smaller min.y change rect_element
               else if(rectangles[rect_element]->min.y < rectangles[i]->min.y)
                  rect_element = i;
            }
			}
			mouse_click = click;
			last.x = x;
			last.y = y;

         // IF in DELETE_MODE
         if(delete_mode)
         {
            if(rect_element != -1)
            {
               // ERASE element rect_element
               rectangles.erase( rectangles.begin() + rect_element);
            }
         }
		}
		else if(state == GLUT_UP)
		{			
			if(rect_element != -1)			
				rectangles[rect_element]->print_rectangle();			
			rect_element = -1;
         intersection = NONE;
		}   
	} 
	glutPostRedisplay();
}

void mouseMovement(int x, int y)
{
   //check the difference between the current x and the last x position
   int diffx = x - last.x;
   //check the difference between the current y and the last y position
   int diffy = y - last.y;
   last.x = x; //set last.x to the current x position
   last.y = y; //set last.y to the current y position
	
   if(rect_element != -1 && !delete_mode)
	{
		// clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw plane
		draw_pushPullPlanes();
		// push or pull face
		new_push_pull( click2point(x, y) );
	}
	else if(firstPoint.x != -1 && !delete_mode)
	{
		// IF this is the first time in this function for this rectangle
		if(counter == 0)
		{
			Point click = click2point(firstPoint.x, firstPoint.y);
			// adjust click for blocksize
			if(click.z < 0)
				click.z = click.z - ((int)click.z % -blocksize);
			else
				click.z = click.z - ((int)click.z % blocksize);			
			if(click.x < 0)
				click.x = click.x - ((int)click.x % -blocksize);
			else
				click.x = click.x - ((int)click.x % blocksize);

			firstPoint = point_to_intPoint(click);
			int building_index = -1;
			// IF firstPoint is on TOP of an existing rectangle3D
			for(int i = 0; i < rectangles.size(); i++)
			{
            if(fabs(rectangles[i]->max.y - click.y) < 5)
				{				
					if(click.z > rectangles[i]->min.z && click.z < rectangles[i]->max.z)
					{
						if(click.x > rectangles[i]->min.x && click.x < rectangles[i]->max.x)						
							building_index = i;
					}
				}
			}
			// IF our rectangle is on top of [building_index] rectangle	
			if(building_index != -1)
			{
				firstPoint.y = rectangles[building_index]->max.y;
				rectangles.push_back( new Rectangle3D( firstPoint, point_to_intPoint( click2point(x, y) ), blocksize ) );
				rectangles[rectangles.size() - 1]->max.y = rectangles[building_index]->max.y;
				rectangles[rectangles.size() - 1]->min.y = rectangles[building_index]->max.y;
				rectangles[rectangles.size() - 1]->print_rectangle();
			}
			// IF our rectangle is not on top of another rectangle
			else
			{
				rectangles.push_back( new Rectangle3D( firstPoint, point_to_intPoint(click2point(x, y)), blocksize) );
				rectangles[rectangles.size() - 1]->max.y = rectangles[rectangles.size() - 1]->min.y = 0;
			}			
			// SET its color
			rectangles[rectangles.size() - 1]->color.red = rand() % 255 / 255.0;
			rectangles[rectangles.size() - 1]->color.green = rand() % 255 / 255.0;
			rectangles[rectangles.size() - 1]->color.blue = rand() % 255 / 255.0;
			//rectangles[rectangles.size() - 1]->adjust_bases();
		}
		// IF this is not the first time in the function for this rectangle
		else
		{
			rectangles[rectangles.size() - 1]->min = firstPoint;
			rectangles[rectangles.size() - 1]->max.x = (point_to_intPoint(click2point(x, y))).x;
			rectangles[rectangles.size() - 1]->max.z = (point_to_intPoint(click2point(x, y))).z;
			rectangles[rectangles.size() - 1]->max.y = rectangles[rectangles.size() - 1]->min.y;
			rectangles[rectangles.size() - 1]->adjust_bases();
		}
      // set rectangle at base of 0 (no negatives!)
		if(rectangles[rectangles.size() - 1]->min.y < 0 || rectangles[rectangles.size() - 1]->max.y < 0)
			rectangles[rectangles.size() - 1]->min.y = rectangles[rectangles.size() - 1]->max.y = 0;
		counter++;

      /* check if outside city radius
      if(rectangles[rectangles.size() - 1]->max.x > city_radius)
         rectangles[rectangles.size() - 1]->max.x = city_radius;
      if(rectangles[rectangles.size() - 1]->max.z > city_radius)
         rectangles[rectangles.size() - 1]->max.z = city_radius;*/
	}
	else if(look_around && mouse_move)
   {
		// change angle      
		theta -= (float) diffx * 0.5;
      phi += (float) diffy * 0.5;
		// update lookAt_pt
		lookAt_pt = polar_to_point(eye_pt, theta, phi, radius);	
		// obtain vectors
		update_vectors();
		//print_camera_variables();
   }
   else if(strafe && mouse_move)
   {
		if(diffy != 0)		
			wads((double(diffy) / double(abs(diffy))), up_vector);
		if(diffx != 0)
			wads(-1 * (double(diffx) / double(abs(diffx))), strafe_vector);
   }
 
   mouse_move = true;
	glutPostRedisplay();
}

// bump up all rectangles on top of rect_index
void recursive_bump(int bottom, int delta_height)
{
	// loop over all rectangles
	int top;
   for(top = 0; top < rectangles.size(); top++)
   {
      // exclude self
      if(top != bottom)
      {
         // IF i.min.y == rect_element.max.y (old_height)
         if( rectangles[top]->min.y == (rectangles[bottom]->max.y - delta_height) )
         {
            // IF rect_element encapsulates i
            if(rectangles[bottom]->encapsulates((rectangles[top]->max), (rectangles[top]->min)))
            {
               // add the difference in heights to i
               rectangles[top]->min.y += delta_height;
               rectangles[top]->max.y += delta_height;
					// call the recursize_bump
					recursive_bump(top, delta_height);
            }
         }
      }
   }
}

void blockalize_face(int index, Face f, bool pull)
{
	if(f == FACE1)
	{
		if(pull)
			rectangles[index]->max.x = rectangles[index]->max.x - ( (rectangles[index]->max.x - rectangles[index]->min.x) % blocksize);
		else
			rectangles[index]->max.x = rectangles[index]->max.x + (blocksize - ( (rectangles[index]->max.x - rectangles[index]->min.x) % blocksize));
	}	
	else if(f == FACE2)
	{	
		if(pull)		
			rectangles[index]->max.z = rectangles[index]->max.z - ( (rectangles[index]->max.z - rectangles[index]->min.z) % blocksize);
		else
			rectangles[index]->max.z = rectangles[index]->max.z + (blocksize - ( (rectangles[index]->max.z - rectangles[index]->min.z) % blocksize));
	}	
	else if(f == FACE3)
	{	
		if(pull)		
			rectangles[index]->min.x = rectangles[index]->min.x + ( (rectangles[index]->max.x - rectangles[index]->min.x) % blocksize);
		else
			rectangles[index]->min.x = rectangles[index]->min.x - (blocksize - ( (rectangles[index]->max.x - rectangles[index]->min.x) % blocksize));
	}	
	else if(f == FACE4)
	{	
		if(pull)		
			rectangles[index]->min.z = rectangles[index]->min.z + ( (rectangles[index]->max.z - rectangles[index]->min.z) % blocksize);
		else
			rectangles[index]->min.z = rectangles[index]->min.z - (blocksize - ( (rectangles[index]->max.z - rectangles[index]->min.z) % blocksize));
	}
}

// re is the rect_element(the rect i'm moving), i is the rectangle being compared to re
bool inPullPath(int reMax, int reMin, int iMax, int iMin)
{
   // IF reMax OR reMin is INSIDE OF iMax AND iMin
   if(reMax < iMax && reMax > iMin || 
      reMin < iMax && reMin > iMin)
   {
      return true;
   }
   
   // IF iMax OR iMin is INSIDE OF reMax AND reMin
   if(iMax < reMax && iMax > reMin ||
      iMin < reMax && iMin > reMin)
   {
      return true;
   }

   return false;
}

// check for rectangle collision
// check for tops going beyond their bottoms
void check_pull(int index, Face f)
{
	for(int i = 0; i < rectangles.size(); i++)
	{
		if(i != index)
		{
			if(f == FACE1)
			{
				// IF i is "in front of" rect_element AND rect_element "passes" another rectangle
	   		if( rectangles[index]->min.x < rectangles[i]->min.x && rectangles[index]->max.x > rectangles[i]->min.x )
		   	{
			  		// check if i is in the path of rect_element
			   	if( inPullPath(rectangles[index]->max.z, rectangles[index]->min.z, rectangles[i]->max.z, rectangles[i]->min.z) )
				   {
					   // set pull extent to collision rectangle's face
   					rectangles[index]->max.x = rectangles[i]->min.x;
	   			}
		   	}
				// IF index is on top of i
				if(rectangles[index]->min.y == rectangles[i]->max.y)
				{
					// IF i encapsulates index
					if(rectangles[i]->semiEncapsulates(FACE1, rectangles[index]->max, rectangles[index]->min))
					{
						// IF face1 of index extends over face1 of i
						if(rectangles[index]->max.x > rectangles[i]->max.x)
							// adjust for constraints
							rectangles[index]->max.x = rectangles[i]->max.x;
					}
				}
			}
			else if(f == FACE2)
			{
				if( rectangles[index]->min.z < rectangles[i]->min.z && rectangles[index]->max.z > rectangles[i]->min.z )
				{
					if( inPullPath(rectangles[index]->max.x, rectangles[index]->min.x, rectangles[i]->max.x, rectangles[i]->min.x) )
						rectangles[index]->max.z = rectangles[i]->min.z;
				}

				if(rectangles[index]->min.y == rectangles[i]->max.y)
				{
					if(rectangles[i]->semiEncapsulates(FACE2, rectangles[index]->max, rectangles[index]->min))
					{
						if(rectangles[index]->max.z > rectangles[i]->max.z)
							rectangles[index]->max.z = rectangles[i]->max.z;
					}
				}
			}			
			else if(f == FACE3)
			{
				if( rectangles[index]->max.x > rectangles[i]->max.x && rectangles[index]->min.x < rectangles[i]->max.x )
				{
					if( inPullPath(rectangles[index]->max.z, rectangles[index]->min.z, rectangles[i]->max.z, rectangles[i]->min.z) )
						rectangles[index]->min.x = rectangles[i]->max.x;
				}

				if(rectangles[index]->min.y == rectangles[i]->max.y)
				{
					if(rectangles[i]->semiEncapsulates(FACE3, rectangles[index]->max, rectangles[index]->min))
					{
						if(rectangles[index]->min.x < rectangles[i]->min.x)
							rectangles[index]->min.x = rectangles[i]->min.x;
					}
				}				
			}
			else if(f == FACE4)
			{
				if( rectangles[index]->max.z > rectangles[i]->max.z && rectangles[index]->min.z < rectangles[i]->max.z )
				{
					if( inPullPath(rectangles[index]->max.x, rectangles[index]->min.x, rectangles[i]->max.x, rectangles[i]->min.x) )
						rectangles[index]->min.z = rectangles[i]->max.z;
				}

				if(rectangles[index]->min.y == rectangles[i]->max.y)
				{
					if(rectangles[i]->semiEncapsulates(FACE4, rectangles[index]->max, rectangles[index]->min))
					{
						if(rectangles[index]->min.z < rectangles[i]->min.z)
							rectangles[index]->min.z = rectangles[i]->min.z;
					}
				}
			}
		}
	}
}

void recursive_push(Face f, int bottom)
{
	int top;
	for(top = 0; top < rectangles.size(); top++)
	{
		// exclude self
      if(top != bottom)
      {         
			// IF i.min.y == rect_element.max.y
         if( rectangles[top]->min.y == rectangles[bottom]->max.y )
         {
				// IF rect_element encapsulates i ON THREE SIDES
            if(rectangles[bottom]->semiEncapsulates(f, (rectangles[top]->max), (rectangles[top]->min)))
            {
					if(rectangles[top]->distance2Face(f, intpoint_to_Point(rectangles[bottom]->whichPoint(f))) < 0)
					{					
						adjust_face(top, f, intpoint_to_Point(rectangles[bottom]->whichPoint(f)), false, false);
						recursive_push(f, top);
					}
				}
			}
		}
	}
}

void adjust_face(int index, Face f, Point mouse_pos, bool block, bool pull)
{
	if(f == FACE1)
	{
		// change face to match mouse position
		rectangles[index]->max.x = mouse_pos.x;
		// IF the max (as a result of the mouse move) is less than the min
		if(rectangles[index]->max.x <= rectangles[index]->min.x)
		{
			// remove the rectangle
			rectangles.erase( rectangles.begin() + index);
			// IF this rectangle is the one that was initially selected from mouse()
			if(index == rect_element)
			{
				// restore initial state
				rect_element = -1;
         	intersection = NONE;
			}
		}
		// IF the max is not less than the min
		else 
		{
			// IF the caller wants this rectangle to be adjusted to blocksize
			if(block)
				blockalize_face(index, f, pull);
			// IF the caller wants to test rectangles above this one (only in the case of a push)
			if(!pull)
				recursive_push(f, index);
			else
				check_pull(index, f);
		}
	}	
	else if(f == FACE2)
	{	
		rectangles[index]->max.z = mouse_pos.z;
		if(rectangles[index]->max.z <= rectangles[index]->min.z)
		{
			rectangles.erase( rectangles.begin() + index);
			if(index == rect_element)
			{
				rect_element = -1;
         	intersection = NONE;
			}
		}
		else 
		{
			if(block)
				blockalize_face(index, f, pull);
			if(!pull)
				recursive_push(f, index);
			else
				check_pull(index, f);
		}
	}
	else if(f == FACE3)
	{		
		rectangles[index]->min.x = mouse_pos.x;
		if(rectangles[index]->min.x >= rectangles[index]->max.x)
		{
			rectangles.erase( rectangles.begin() + index);
			if(index == rect_element)
			{
				rect_element = -1;
         	intersection = NONE;
			}
		}
		else 
		{
			if(block)
				blockalize_face(index, f, pull);
			if(!pull)
				recursive_push(f, index);
			else
				check_pull(index, f);
		}
	}	
	else if(f == FACE4)
	{
		rectangles[index]->min.z = mouse_pos.z;
		if(rectangles[index]->min.z >= rectangles[index]->max.z)
		{
			rectangles.erase( rectangles.begin() + index);
			if(index == rect_element)
			{
				rect_element = -1;
         	intersection = NONE;
			}
		}
		else 
		{
			if(block)
				blockalize_face(index, f, pull);
			if(!pull)
				recursive_push(f, index);
			else
				check_pull(index, f);
		}
	}
}

void new_push_pull(Point mouse_pos)
{
	if(intersection == TOP)
   {
      // move rectangles on top of this rectangle too
      // record old height
      int old_height = rectangles[rect_element]->max.y;
      // update height
      rectangles[rect_element]->max.y = mouse_pos.y;
		// max.y cannot be < min.y
      if(rectangles[rect_element]->max.y < rectangles[rect_element]->min.y)
         rectangles[rect_element]->max.y = rectangles[rect_element]->min.y;
		// adjust for blocksize
		rectangles[rect_element]->max.y = rectangles[rect_element]->max.y - (rectangles[rect_element]->max.y % blocksize);
		if(old_height != rectangles[rect_element]->max.y)
			recursive_bump(rect_element, rectangles[rect_element]->max.y - old_height);
   }

	// see if mouse_pos passes blocksize thresh hold
	if(abs(rectangles[rect_element]->distance2Face(intersection, mouse_pos)) > blocksize)
	{	
		// IF PUSH
		if(rectangles[rect_element]->distance2Face(intersection, mouse_pos) < 0)
			adjust_face(rect_element, intersection, mouse_pos, true, false);
		
		// IF PULL
		else
			adjust_face(rect_element, intersection, mouse_pos, true, true);
	}
}

void draw_rectangles()
{
	for(int i = 0; i < rectangles.size(); i++)
		rectangles[i]->draw();
}

// draws xz grid
void draw_grid()
{
   // draw circle
   int i, sections = 40;
   GLfloat twoPi = 2.0 * 3.14159;

   glBegin(GL_TRIANGLE_FAN);
   glColor3f(1, 1, 1);
   glVertex3f(0, -0.5, 0);
   for(i = 0; i <= sections; i++)
      glVertex3f(city_radius * cos(i * twoPi / sections), -0.5, city_radius * sin(i * twoPi / sections));
   glEnd();
   
   radius = 200;
   glBegin(GL_TRIANGLE_FAN);
   glColor3f(0, 0, 0);
   glVertex3f(0, -0.6, 0);
   for(i = 0; i <= sections; i++)
   {
      glVertex3f((city_radius + 50) * cos(i * twoPi / sections), -0.6, (city_radius + 50) * sin(i * twoPi / sections));
   }
   glEnd();
}

void draw_pushPullPlanes()
{
	int i, sections = 40;
	GLfloat twoPi = 2.0 * 3.14159;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);
	if(intersection == TOP)
	{
		// x fan
		glVertex3f(mouse_click.x, 0, 0);
		for(i = 0; i <= sections; i++)
			glVertex3f(mouse_click.x, 3 * city_radius * cos(i * twoPi / sections), 3 * city_radius * sin(i * twoPi / sections));
		glEnd();
		// z fan
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, mouse_click.z);
		for(i = 0; i <= sections; i++)
			glVertex3f(3 * city_radius * cos(i * twoPi / sections), 3 * city_radius * sin(i * twoPi / sections), mouse_click.z);
	}
	else
	{
		glVertex3f(0, mouse_click.y, 0);
		for(i = 0; i <= sections; i++)
			glVertex3f(3 * city_radius * cos(i * twoPi / sections), mouse_click.y, 3 * city_radius * sin(i * twoPi / sections));
	}
	glEnd();
}

void print_rectangles()
{
   printf("***********\n");
   for(int i = 0; i < rectangles.size(); i++)
      rectangles[i]->print_rectangle();
}

void display()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
   glMatrixMode(GL_MODELVIEW);
	gluLookAt(eye_pt.x, eye_pt.y, eye_pt.z, 
             lookAt_pt.x, lookAt_pt.y, lookAt_pt.z, 
             up_vector.head.x, up_vector.head.y, up_vector.head.z);

   // toggle axis
	if(draw_axis < 0)
	   draw_xyz_axis();	
	draw_grid();
	enable();
	pos_light();

	draw_rectangles();

	glutSwapBuffers();
}

void init_globals()
{
   enable();
   // eye and lookAt positions
	// this establishes a permanent radius of 40
	eye_pt.x = 0;
	eye_pt.y = 10;
	eye_pt.z = -40; 
	lookAt_pt.x = 0;
	lookAt_pt.y = 10;
	lookAt_pt.z = 0;
	// set initial polar coordinates relating the eye to the lookAt point
	theta = 180;
	phi = 90;
	radius = 40;
	// vector math
	update_vectors();
	//print_camera_variables();
	// object transform variable initialization
	// seed random	
	srand ( static_cast<unsigned int> ( time ( 0 ) ) );	
	
   glutPostRedisplay();
}

int main( int argc, char** argv )
{
	int menu;   
	//initialize  globals
   GW = 600;
   GH = 600;	
	//set up my window
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(600, 600); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("BuildIt!");
   glClearColor(1.0, 1.0, 1.0, 1.0);
   
   init_globals();

   //register glut callback functions
   glutDisplayFunc( display );
   glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
   //glutPassiveMouseFunc( passiveMouse );
   glutMotionFunc( mouseMovement );
	glutKeyboardFunc (keyboard);

   glutMainLoop();
}
