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
GLUquadricObj* mySphere;
int city_radius = 150;
int blocksize = 10;

// boolean variables
int draw_axis = 1, rect_element = -1;
bool delete_mode = false, look_around = false, mouse_move = false, strafe = false, pauseGame = false, circles = true;
Face intercetion = NONE;
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
void draw_pushPullPlanes();
void print_rectangles();

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

void readRectsFromFile(string filename)
{
   // clear rectangles vector
   rectangles.clear();
   // add rectangles to vector 
   string line;
   ifstream myfile (filename.c_str());
   if(myfile.is_open())
   {
      while(!myfile.eof())
      {
         // create a new rectangle
         Rectangle3D* rect = new Rectangle3D();
         // read one line at a time
         myfile >> rect->min.x >> rect->min.y >> rect->min.z >> rect->max.x >> rect->max.y >> rect->max.z;
         // push_back rectangle
         rectangles.push_back(rect);
      }
      myfile.close();
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
	
	//print_camera_variables();	
	// check if eye is under water
	/*if(eye_pt.z < 1)
	{
		eye_pt.z = 1;
		lookAt_pt.z = 1;
	}*/
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

      // read rectangles from file and add to rectangles vector
      case 'r':
         printf("READ FROM FILE: ");
         scanf("%s", filename);
         readRectsFromFile(filename);
         break;

      // change the blocksize
      case 'b':
         printf("NEW BLOCKSIZE: ");
         scanf("%d", &blocksize);
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
			/*rectangles[rectangles.size() - 1]->min = firstPoint;
			rectangles[rectangles.size() - 1]->max.x = (point_to_intPoint(click2point(x, y)).x);
			rectangles[rectangles.size() - 1]->max.z = (point_to_intPoint(click2point(x, y)).z);
			// adjust click for blocksize
			if(rectangles[rectangles.size() - 1]->max.z < 0)
				rectangles[rectangles.size() - 1]->max.z = rectangles[rectangles.size() - 1]->max.z - ((int)rectangles[rectangles.size() - 1]->max.z % -blocksize);
			else
				rectangles[rectangles.size() - 1]->max.z = rectangles[rectangles.size() - 1]->max.z - ((int)rectangles[rectangles.size() - 1]->max.z % blocksize);			
			if(rectangles[rectangles.size() - 1]->max.x < 0)
				rectangles[rectangles.size() - 1]->max.x = rectangles[rectangles.size() - 1]->max.x - ((int)rectangles[rectangles.size() - 1]->max.x % -blocksize);
			else
				rectangles[rectangles.size() - 1]->max.x = rectangles[rectangles.size() - 1]->max.x - ((int)rectangles[rectangles.size() - 1]->max.x % blocksize);
			rectangles[rectangles.size() - 1]->max.y = rectangles[rectangles.size() - 1]->min.y;
			rectangles[rectangles.size() - 1]->adjust_bases();*/

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
					if(intercetion == NONE)
               {
                  intercetion = temp_face;
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
         intercetion = NONE;
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
		push_pull( click2point(x, y) );
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

void passiveMouse(int x, int y)
{
   /*if(delete_mode)
   {
      static POINT lastMouse = {0,0};
      GetCursorPos(&lastMouse);
      SetCursorPos(320, 240);//what?
      float moveX = float((320 - lastMouse.x)) / 100.0f;
      float moveY = float((240 - lastMouse.y)) / 100.0f;
      mouse_pos.x -= moveX;
      mouse_pos.y += moveY;
   }*/
}

// re is the rect_element, i is the rectangle being compared to re
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

// bump up all rectangles on top of rect_index
void recursive_bump(int rect_index, int delta_height)
{
	// loop over all rectangles
	int i;
   for(i = 0; i < rectangles.size(); i++)
   {
      // exclude self
      if(i != rect_element)
      {
         // IF i.min.y == rect_element.max.y (old_height)
         if( rectangles[i]->min.y == (rectangles[rect_index]->max.y - delta_height) )
         {
            // IF rect_element encapsulates i
            if(rectangles[rect_element]->encapsulates((rectangles[i]->max), (rectangles[i]->min)))
            {
               // add the difference in heights to i
               rectangles[i]->min.y += delta_height;
               rectangles[i]->max.y += delta_height;
					// call the recursize_bump
					recursive_bump(i, delta_height);
            }
         }
      }
   }
}

void push_pull(Point mouse_pos)
{
   int i;
	// change in y
   if(intercetion == TOP)
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
   // change in x
   else if(intercetion == FACE1)
   {
      // record old face1
      int old_face1 = rectangles[rect_element]->max.x;
      //update face1
      rectangles[rect_element]->max.x = mouse_pos.x;
		// max.x !< min.x
		if(rectangles[rect_element]->max.x < rectangles[rect_element]->min.x)
         rectangles[rect_element]->max.x = rectangles[rect_element]->min.x;
		// now test rect collsion and rects ontop of this one
		for(i = 0; i < rectangles.size(); i++)
		{
			// exclude self
			if(i != rect_element)
			{
            // IF i is on top of rect_element
				if(rectangles[rect_element]->max.y == rectangles[i]->min.y)
            {
               // IF their face1's are the same
               if(rectangles[i]->max.x == old_face1)
               {
                  // IF pushing face in
                  if(old_face1 > rectangles[rect_element]->max.x)
                     rectangles[i]->max.x = rectangles[rect_element]->max.x;
                  // IF max.x < min.x, erase
                  if(rectangles[i]->max.x < rectangles[i]->min.x)
                     rectangles.erase( rectangles.begin() + i);
               }
            }
            else
            {
               // IF i is "in front of" rect_element AND rect_element "passes" another rectangle
	   			if( rectangles[rect_element]->min.x < rectangles[i]->min.x && rectangles[rect_element]->max.x > rectangles[i]->min.x )
		   		{
			   		// check if i is in the path of rect_element
				   	if( inPullPath(rectangles[rect_element]->max.z, rectangles[rect_element]->min.z, rectangles[i]->max.z, rectangles[i]->min.z) )
					   {
						   // set pull extent to collision rectangle's face
   						rectangles[rect_element]->max.x = rectangles[i]->min.x;
	   				}
		   		}
			   }
         }
		}
		// adjust for blocksize
		rectangles[rect_element]->max.x = rectangles[rect_element]->max.x - ( (rectangles[rect_element]->max.x - rectangles[rect_element]->min.x) % blocksize);
   }
   // change in z
   else if(intercetion == FACE2)
   {
		// record old face2
      int old_face2 = rectangles[rect_element]->max.z;
      //update face2      
		rectangles[rect_element]->max.z = mouse_pos.z;
		// max.z !< min.z
      if(rectangles[rect_element]->max.z < rectangles[rect_element]->min.z)
         rectangles[rect_element]->max.z = rectangles[rect_element]->min.z;
		// now test rect collsion
		for(i = 0; i < rectangles.size(); i++)
		{
			// exclude self
			if(i != rect_element)
			{
				// IF i is on top of rect_element
				if(rectangles[rect_element]->max.y == rectangles[i]->min.y)
            {
               // IF their face2's are the same
               if(rectangles[i]->max.z == old_face2)
               {
                  // IF pushing face in
                  if(old_face2 > rectangles[rect_element]->max.z) 
                     rectangles[i]->max.z = rectangles[rect_element]->max.z;
                  // IF max.z < min.z, erase
                  if(rectangles[i]->max.z < rectangles[i]->min.z)
                     rectangles.erase( rectangles.begin() + i);
               }
            }				
				else
				{
					// IF i is "in front of" rect_element AND rect_element "passes" another rectangle
					if( rectangles[rect_element]->min.z < rectangles[i]->min.z && rectangles[rect_element]->max.z > rectangles[i]->min.z )
					{
						// check if i is in the path of rect_element
						if( inPullPath(rectangles[rect_element]->max.x, rectangles[rect_element]->min.x, rectangles[i]->max.x, rectangles[i]->min.x) )
						{
							// set pull extent to collision rectangle's face
							rectangles[rect_element]->max.z = rectangles[i]->min.z;
						}
					}
				}
			}
		}
		// adjust for blocksize
		rectangles[rect_element]->max.z = rectangles[rect_element]->max.z - ( (rectangles[rect_element]->max.z - rectangles[rect_element]->min.z) % blocksize);
   }
   // change in x
   else if(intercetion == FACE3)
   {
		// record old face3
      int old_face3 = rectangles[rect_element]->min.x;
      //update face3 
		rectangles[rect_element]->min.x = mouse_pos.x;
		// min.x !> max.x
      if(rectangles[rect_element]->min.x > rectangles[rect_element]->max.x)
         rectangles[rect_element]->min.x = rectangles[rect_element]->max.x;
		// now test rect collsion
		for(i = 0; i < rectangles.size(); i++)
		{
			// exclude self
			if(i != rect_element)
			{
				// IF i is on top of rect_element
				if(rectangles[rect_element]->max.y == rectangles[i]->min.y)
            {
               // IF their face1's are the same
               if(rectangles[i]->min.x == old_face3)
               {
                  // IF pushing face in
                  if(old_face3 < rectangles[rect_element]->min.x)
                     rectangles[i]->min.x = rectangles[rect_element]->min.x;
                  // IF min.x > max.x, erase
                  if(rectangles[i]->min.x > rectangles[i]->max.x)
                     rectangles.erase( rectangles.begin() + i);
               }
            }
            else
            {				
					// IF i is "in front of" rect_element AND rect_element "passes" another rectangle
					if( rectangles[rect_element]->max.x > rectangles[i]->max.x && rectangles[rect_element]->min.x < rectangles[i]->max.x )
					{
						// check if i is in the path of rect_element
						if( inPullPath(rectangles[rect_element]->max.z, rectangles[rect_element]->min.z, rectangles[i]->max.z, rectangles[i]->min.z) )
						{
							// set pull extent to collision rectangle's face
							rectangles[rect_element]->min.x = rectangles[i]->max.x;
						}
					}
				}
			}
		}
		// adjust for blocksize
		rectangles[rect_element]->min.x = rectangles[rect_element]->min.x + ( (rectangles[rect_element]->max.x - rectangles[rect_element]->min.x) % blocksize);
   }
   // change in z
   else if(intercetion == FACE4)
   {
		// record old face4
      int old_face4 = rectangles[rect_element]->min.z;
      //update face4      
		rectangles[rect_element]->min.z = mouse_pos.z;
		// min.z !> max.z
      if(rectangles[rect_element]->min.z > rectangles[rect_element]->max.z)
         rectangles[rect_element]->min.z = rectangles[rect_element]->max.z;
		// now test rect collsion
		for(i = 0; i < rectangles.size(); i++)
		{
			// exclude self
			if(i != rect_element)
			{
				// IF i is on top of rect_element
				if(rectangles[rect_element]->max.y == rectangles[i]->min.y)
            {
               // IF their face2's are the same
               if(rectangles[i]->min.z == old_face4)
               {
                  // IF pushing face in
                  if(old_face4 < rectangles[rect_element]->min.z)
                     rectangles[i]->min.z = rectangles[rect_element]->min.z;
                  // IF min.z > max.z, erase
                  if(rectangles[i]->min.z > rectangles[i]->max.z)
                     rectangles.erase( rectangles.begin() + i);
               }
            }				
				else
				{				
					// IF i is "in front of" rect_element AND rect_element "passes" another rectangle
					if( rectangles[rect_element]->max.z > rectangles[i]->max.z && rectangles[rect_element]->min.z < rectangles[i]->max.z )
					{
						// check if i is in the path of rect_element
						if( inPullPath(rectangles[rect_element]->max.x, rectangles[rect_element]->min.x, rectangles[i]->max.x, rectangles[i]->min.x) )
						{
							// set pull extent to collision rectangle's face
							rectangles[rect_element]->min.z = rectangles[i]->max.z;
						}
					}
				}
			}
		}
		// adjust for blocksize
		rectangles[rect_element]->min.z = rectangles[rect_element]->min.z + ( (rectangles[rect_element]->max.z - rectangles[rect_element]->min.z) % blocksize);
   }
   // change in y
   else if(intercetion == BOTTOM)
   {
	   rectangles[rect_element]->min.y = mouse_pos.y;
		// min.y !< 0
      if(rectangles[rect_element]->min.y < 0)
         rectangles[rect_element]->min.y = 0;
   }
   //print_rectangles();
}

void draw_mouse_cursor()
{
	//Will be transparent
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   
   //Draw the cursor at the current mouse pos
   glBegin(GL_TRIANGLES);
     glColor4f(0.75,0.75,0.75,0.75);
     glVertex3f(mouse_pos.x, mouse_pos.y, DEPTH);
     glVertex3f(mouse_pos.x, mouse_pos.y - 0.5f, DEPTH);
     glVertex3f(mouse_pos.x + 0.5f, mouse_pos.y, DEPTH);
   glEnd();
   
   //Alles wird wie vorher
   glDisable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
}

void draw_points()
{
	glColor3f(119.0 / 255, 136.0 / 255, 153.0 / 255);	
	for(int i = 0; i < points.size(); i++)
	{
		glPushMatrix();
   	glTranslated(points[i].x, points[i].y, points[i].z);
		glutSolidSphere(2, 20, 20);
		glPopMatrix();
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
   /*glEnable(GL_CULL_FACE);
   glCullFace(GL_FRONT);
   glBindTexture(GL_TEXTURE_2D, 1);
   glColor3f(1, 1, 1);
   gluSphere(mySphere, 150.0, 20, 20);
   glDisable(GL_CULL_FACE);*/

   // draw circle
   int i, sections = 40;
   GLfloat twoPi = 2.0 * 3.14159;

   glBegin(GL_TRIANGLE_FAN);
   glColor3f(1, 1, 1);
   glVertex3f(0, -0.5, 0);
   for(i = 0; i <= sections; i++)
   {
      glVertex3f(city_radius * cos(i * twoPi / sections), -0.5, city_radius * sin(i * twoPi / sections));
   }
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
	if(intercetion == TOP)
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
   
   // load textures
   //LoadTexture("skyline2.bmp", 1);
   
   // create a sphere
   mySphere = gluNewQuadric();
   gluQuadricDrawStyle(mySphere, GLU_FILL);
   // make opengl generate the texture coordinates for the sphere
   gluQuadricTexture(mySphere, GL_TRUE);

   //register glut callback functions
   glutDisplayFunc( display );
   glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
   //glutPassiveMouseFunc( passiveMouse );
   glutMotionFunc( mouseMovement );
	glutKeyboardFunc (keyboard);

   glutMainLoop();
}
