#include <map>
#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <math.h>

#include "BuildState.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../graphics/orbitalcamera.h"
#include "../helper/Vector.h"
#include "../graphics/graphics.h"
#include "../scene/CustomObject.h"

namespace BuildStateGlobals
{
	bool DELETE_MODE;
	bool MOUSE_DOWN;
	int LAST_BUTTON;
	int blocksize, pp_index, counter;
	Face pp_face;
	Point firstPoint, last, mouse_click;
}

#define ANGLE_SPEED 2
#define MOUSE_SPEED 1
#define STRAFE_SPEED 20
#define PI 3.1415
#define DIST 30

using namespace io;
using namespace global;
using namespace BuildStateGlobals;

BuildState::BuildState() {
   initialize();
}

BuildState::~BuildState() {}

void BuildState::initialize() {
	DELETE_MODE = false;
	MOUSE_DOWN = false;
	counter = 0;
	blocksize = 5;
	pp_index = -1;
	pp_face = NOTHING;
	firstPoint = Point();

	// create the camera
	if (global::camera != NULL) delete global::camera;
	global::camera = new OrbitalCamera();
	global::camera->init(Vector(30.0, 30.0, 30.0), Vector(0.0, 5.0, 0.0));
	OrbitalCamera *cam = static_cast<OrbitalCamera *>(global::camera);
	cam->shiftCaptureMode(true);

	// mouse should be free to click
	io::release_mouse();

	io::register_mouse_down(BuildState::mouseDownToggle);
	io::register_mouse_up(BuildState::mouseUpToggle);
	io::register_key_down(BuildState::keyDownHandler);

	// make a simple CustomObject and add it to static_cast<CustomObject*>(objects vector
	// (unsigned int newid, unsigned int newplayerid, ObjectType newtype, Point newmax, Point newmin)
	//CustomObject co = new CustomObject(0, 0, CUSTOM_BLOCK, Point(25, 0, 25), Point(-25, 25, -25));
	objects.push_back(new CustomObject(0, 0, CUSTOM_BLOCK, Point(5, 10, 5), Point(-5, 0, -5)));
}

void BuildState::update(long milli_time) {
   updateInput(milli_time);
}

void BuildState::updateInput(long milli_time) {
   // General Mouse Layout
	if(MOUSE_DOWN)
		mouseDownHandler();

	// General Keyboard Layout
	if(io::keys[27]) {
		exit(0);
	}

   if(io::keys['f']) {
	   if (io::captured)
		{
			io::release_mouse();
	   }
		else
		{
			io::capture_mouse();
	   }
   }
}

void BuildState::keyDownHandler(int key, bool special)
{
	/*// General Keyboard Layout
	if(key == 27) {
		exit(0);
	}

   if(key == 'f') {
	   if (io::captured) {
		   io::release_mouse();
	   } else {
		   io::capture_mouse();
	   }
   }*/
}

// called only on the first click down
void BuildState::mouseDownToggle(int button)
{
	LAST_BUTTON = button;
	MOUSE_DOWN = true;
	firstPoint.set(Point(io::mouse_x, io::mouse_y));
	//printf("***\nfristPoint: %s\n", firstPoint.str());
}

// called only on the first mouse up
void BuildState::mouseUpToggle(int button)
{
	InGameState *currState = global::stateManager->currentState;
	//printf("last click\n");
	if(button == GLUT_RIGHT_BUTTON)
		static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
	LAST_BUTTON = button;
	MOUSE_DOWN = false;
	counter = 0;
	pp_face = NOTHING;
	pp_index = -1;
}

void BuildState::mouseDownHandler()
{
	InGameState *currState = global::stateManager->currentState;

	// IF firstPoint hasn't been evaluated yet
	if(counter == 0)
		// check if firstPoint intersects with any existing rectangles
		evaluateClick(firstPoint);

	// IF right click
	if(LAST_BUTTON == GLUT_RIGHT_BUTTON)
	{
		// IF first click
		if(counter == 0)
		{
			// blockalize first point
			firstPoint.adjustPointForBlocksize(blocksize);			
			
			// record point
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.adjustPointForBlocksize(blocksize);
			// push_back new CustomObject
			currState->objects.push_back( new CustomObject( currState->objects.size(), 0, CUSTOM_BLOCK, firstPoint, Point(io::mouse_x, io::mouse_y) ) );
			placeY(currState->objects.size() - 1, pp_index);
		}
		// IF not first click
		else
		{
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.adjustPointForBlocksize(blocksize);
			//printf("counter %d\n", counter);	
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_min(firstPoint);
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_x(click.getx());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_z(click.getz());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_y(static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->get_min_y());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->adjust_bases();
			placeY(currState->objects.size() - 1, -1);
			//static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
		}
		counter++;
	}
	// IF left click
	else if(LAST_BUTTON == GLUT_LEFT_BUTTON)
	{
		// IF mouse has been left clicked on an existing rectangle
		if(pp_index != -1)
		{
			// get push/pull plane
			printf("get pp planes!\n");
			get_pp_plane();			
			new_push_pull(Point(io::mouse_x, io::mouse_y));
		}
		counter++;
	}
	
}

// given the current location of the mouse and the pp_face 
// determines the plane that gluUnproject samples against
void BuildState::get_pp_plane()
{
	// Vertical	pushPull
	if(pp_face == TOP)
	{
		// obtain a plane perpendicular to the view vector
		float offsetSize = 10;
		Vector v = global::camera->viewVec() * -1;
		Vector offset = global::camera->strafeVec();//Vector.crossProduct(v, Vector(0,1,0));
		offset.y(1);
		offset = offset * offsetSize;
		
		Point p;
		p.set(Point(io::mouse_x, io::mouse_y));
		
		glBegin(GL_QUADS);
		
			glVertex3f(p.getx() + offset.x(), p.gety() + offset.y(), p.getz() + offset.z());
			glVertex3f(p.getx() - offset.x(), p.gety() + offset.y(), p.getz() - offset.z());
			glVertex3f(p.getx() - offset.x(), p.gety() - offset.y(), p.getz() - offset.z());
			glVertex3f(p.getx() + offset.x(), p.gety() - offset.y(), p.getz() + offset.z());
		
		glEnd();
		printf("get_pp_planes end!\n");
	}
	// Horizontal pushPull
	else if(pp_face != NOTHING)
	{
		float offset = 50;
		Point p;
		p.set(Point(io::mouse_x, io::mouse_y));

		glBegin(GL_QUADS);
		
			glVertex3f(p.getx() + offset, p.gety(), p.getz() + offset);
			glVertex3f(p.getx() - offset, p.gety(), p.getz() - offset);
			glVertex3f(p.getx() - offset, p.gety(), p.getz() - offset);
			glVertex3f(p.getx() + offset, p.gety(), p.getz() + offset);
		
		glEnd();
	}
}

/*void draw_pushPullPlanes()
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
}*/

// determines if a mouse click intersects with an existing rectangle
// if it does, it sets global variables pp_index (the index of the rectangle it intersects with)
// and pp_face (the face of the rectangle it intersects with)
void BuildState::evaluateClick(Point click)
{
	InGameState *currState = global::stateManager->currentState;
	int i = 0;	
	//printf("-------\n");
	// FOR all objects
	while (i < currState->objects.size() && pp_face == NOTHING)
	{
		pp_face = static_cast<CustomObject*>(objects[i])->check_click(click);
		if(pp_face != NOTHING)
			pp_index = i;
		i++;
	}
	
	/*if(pp_face == NOTHING)
		printf("clicked nothing, currState->objects.size() = %d\n", currState->objects.size());
	else
		printf("clicked rectangle %d's face %d\n", pp_index, pp_face);*/
}

void BuildState::placeY(int rect_index, int below_index)
{
	// IF rect_index is on top of another rectangle
	if(below_index != -1)
	{
		static_cast<CustomObject*>(objects[rect_index])->set_min_y(static_cast<CustomObject*>(objects[rect_index])->get_max_y());
		static_cast<CustomObject*>(objects[rect_index])->set_max_y(static_cast<CustomObject*>(objects[rect_index])->get_max_y());
	}
	// IF rect_index is on the ground plane
	else
	{
		static_cast<CustomObject*>(objects[rect_index])->set_min_y(0);
		static_cast<CustomObject*>(objects[rect_index])->set_max_y(0);
	}
}

// bump up all static_cast<CustomObject*>(objects on top of rect_index
void BuildState::recursive_bump(int bottom, int delta_height)
{
	// loop over all static_cast<CustomObject*>(objects
	int top;
   for(top = 0; top < objects.size(); top++)
   {
      // exclude self
      if(top != bottom)
      {
         // IF i.get_min_y() == pp_index.get_max_y() (old_height)
         if( static_cast<CustomObject*>(objects[top])->set_min_y( (static_cast<CustomObject*>(objects[bottom])->get_max_y() - delta_height) ) )
         {
            // IF pp_index encapsulates i
            if(static_cast<CustomObject*>(objects[bottom])->encapsulates((static_cast<CustomObject*>(objects[top])->get_max()), (static_cast<CustomObject*>(objects[top])->get_min())))
            {
               // add the difference in heights to i
               static_cast<CustomObject*>(objects[top])->set_min_y( static_cast<CustomObject*>(objects[top])->get_min_y() + delta_height );
               static_cast<CustomObject*>(objects[top])->set_max_y( static_cast<CustomObject*>(objects[top])->get_max_y() + delta_height );
					// call the recursize_bump
					recursive_bump(top, delta_height);
            }
         }
      }
   }
}

void BuildState::blockalize_face(int index, Face f, bool pull)
{
	if(f == FACE1)
	{
		if(pull)
			static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() - ( (static_cast<CustomObject*>(objects[index])->get_max_x() - static_cast<CustomObject*>(objects[index])->get_min_x()) % blocksize));
		else
			static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() + (blocksize - ( (static_cast<CustomObject*>(objects[index])->get_max_x() - static_cast<CustomObject*>(objects[index])->get_min_x()) % blocksize)));
	}
	else if(f == FACE2)
	{
		if(pull)
			static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() - ( (static_cast<CustomObject*>(objects[index])->get_max_z() - static_cast<CustomObject*>(objects[index])->get_min_z()) % blocksize));
		else
			static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() + (blocksize - ( (static_cast<CustomObject*>(objects[index])->get_max_z() - static_cast<CustomObject*>(objects[index])->get_min_z()) % blocksize)));
	}
	else if(f == FACE3)
	{
		if(pull)
			static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() + ( (static_cast<CustomObject*>(objects[index])->get_max_x() - static_cast<CustomObject*>(objects[index])->get_min_x()) % blocksize));
		else
			static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() - (blocksize - ( (static_cast<CustomObject*>(objects[index])->get_max_x() - static_cast<CustomObject*>(objects[index])->get_min_x()) % blocksize)));
	}
	else if(f == FACE4)
	{
		if(pull)
			static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() + ( (static_cast<CustomObject*>(objects[index])->get_max_z() - static_cast<CustomObject*>(objects[index])->get_min_z()) % blocksize));
		else
			static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() - (blocksize - ( (static_cast<CustomObject*>(objects[index])->get_max_z() - static_cast<CustomObject*>(objects[index])->get_min_z()) % blocksize)));
	}
}

// re is the pp_index(the rect i'm moving), i is the rectangle being compared to re
bool BuildState::inPullPath(int reMax, int reMin, int iMax, int iMin)
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
void BuildState::check_pull(int index, Face f)
{
	for(int i = 0; i < objects.size(); i++)
	{
		if(i != index)
		{
			if(f == FACE1)
			{
				// IF i is "in front of" pp_index AND pp_index "passes" another rectangle
	   		if( static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_min_x() && static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_min_x() )
		   	{
					// check if i is not on the same "level" as index
					if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					{
						// check if i is in the path of pp_index
				   	if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_z(), static_cast<CustomObject*>(objects[index])->get_min_z(), static_cast<CustomObject*>(objects[i])->get_max_z(), static_cast<CustomObject*>(objects[i])->get_min_z()) )
					   {
						   // set pull extent to collision rectangle's face
   						static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[i])->get_min_x());
	   				}
					}
		   	}
				// IF index is on top of i
				if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					// IF i encapsulates index
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE1, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						// IF face1 of index extends over face1 of i
						if(static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_max_x())
							// adjust for constraints
							static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[i])->get_max_x());
					}
				}
			}
			else if(f == FACE2)
			{
				if( static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_min_z() && static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_min_z() )
				{
					if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					{
						if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_x(), static_cast<CustomObject*>(objects[index])->get_min_x(), static_cast<CustomObject*>(objects[i])->get_max_x(), static_cast<CustomObject*>(objects[i])->get_min_x()) )
							static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[i])->get_min_z());
					}
				}

				if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE2, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						if(static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_max_z())
							static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[i])->get_max_z());
					}
				}
			}
			else if(f == FACE3)
			{
				if( static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_max_x() && static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_max_x() )
				{
					if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					{
						if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_z(), static_cast<CustomObject*>(objects[index])->get_min_z(), static_cast<CustomObject*>(objects[i])->get_max_z(), static_cast<CustomObject*>(objects[i])->get_min_z()) )
							static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[i])->get_max_x());
					}
				}

				if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE3, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						if(static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_min_x())
							static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[i])->get_min_x());
					}
				}
			}
			else if(f == FACE4)
			{
				if( static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_max_z() && static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_max_z() )
				{
					if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					{
						if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_x(), static_cast<CustomObject*>(objects[index])->get_min_x(), static_cast<CustomObject*>(objects[i])->get_max_x(), static_cast<CustomObject*>(objects[i])->get_min_x()) )
							static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[i])->get_max_z());
					}
				}

				if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE4, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						if(static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_min_z())
							static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[i])->get_min_z());
					}
				}
			}
		}
	}
}

void BuildState::recursive_push(Face f, int bottom)
{
	int top;
	for(top = 0; top < objects.size(); top++)
	{
		// exclude self
      if(top != bottom)
      {
			// IF i.get_min_y() == pp_index.get_max_y()
         if( static_cast<CustomObject*>(objects[top])->get_min_y() == static_cast<CustomObject*>(objects[bottom])->get_max_y() )
         {
				// IF pp_index encapsulates i ON THREE SIDES
            if(static_cast<CustomObject*>(objects[bottom])->semiEncapsulates(f, (static_cast<CustomObject*>(objects[top])->get_max()), (static_cast<CustomObject*>(objects[top])->get_min())))
            {
					if(static_cast<CustomObject*>(objects[top])->distance2Face(f, static_cast<CustomObject*>(objects[bottom])->whichPoint(f)) < 0)
					{
						adjust_face(top, f, static_cast<CustomObject*>(objects[bottom])->whichPoint(f), false, false);
						recursive_push(f, top);
					}
				}
			}
		}
	}
}

void BuildState::adjust_face(int index, Face f, Point mouse_pos, bool block, bool pull)
{
	if(f == FACE1)
	{
		// change face to match mouse position
		static_cast<CustomObject*>(objects[index])->set_max_x(mouse_pos.getx());
		// IF the max (as a result of the mouse move) is less than the min
		if(static_cast<CustomObject*>(objects[index])->get_max_x() <= static_cast<CustomObject*>(objects[index])->get_min_x())
		{
			// remove the rectangle
			objects.erase( objects.begin() + index);
			// IF this rectangle is the one that was initially selected from mouse()
			if(index == pp_index)
			{
				// restore initial state
				pp_index = -1;
         	pp_face = NOTHING;
			}
		}
		// IF the max is not less than the min
		else
		{
			// IF the caller wants this rectangle to be adjusted to blocksize
			if(block)
				blockalize_face(index, f, pull);
			// IF the caller wants to test static_cast<CustomObject*>(objects above this one (only in the case of a push)
			if(!pull)
				recursive_push(f, index);
			else
				check_pull(index, f);
		}
	}
	else if(f == FACE2)
	{
		static_cast<CustomObject*>(objects[index])->set_max_z(mouse_pos.getz());
		if(static_cast<CustomObject*>(objects[index])->get_max_z() <= static_cast<CustomObject*>(objects[index])->get_min_z())
		{
			objects.erase(objects.begin() + index);
			if(index == pp_index)
			{
				pp_index = -1;
         	pp_face = NOTHING;
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
		static_cast<CustomObject*>(objects[index])->set_min_x(mouse_pos.getx());
		if(static_cast<CustomObject*>(objects[index])->get_min_x() >= static_cast<CustomObject*>(objects[index])->get_max_x())
		{
			objects.erase(objects.begin() + index);
			if(index == pp_index)
			{
				pp_index = -1;
         	pp_face = NOTHING;
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
		static_cast<CustomObject*>(objects[index])->set_min_z(mouse_pos.getz());
		if(static_cast<CustomObject*>(objects[index])->get_min_z() >= static_cast<CustomObject*>(objects[index])->get_max_z())
		{
			objects.erase(objects.begin() + index);
			if(index == pp_index)
			{
				pp_index = -1;
         	pp_face = NOTHING;
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

void BuildState::new_push_pull(Point mouse_pos)
{
	if(pp_face == TOP)
   {
      // move static_cast<CustomObject*>(objects on top of this rectangle too
      // record old height
      int old_height = static_cast<CustomObject*>(objects[pp_index])->get_max_y();
      // update height
      static_cast<CustomObject*>(objects[pp_index])->set_max_y(mouse_pos.gety());
		// get_max_y() cannot be < get_min_y()
      if(static_cast<CustomObject*>(objects[pp_index])->get_max_y() < static_cast<CustomObject*>(objects[pp_index])->get_min_y())
         static_cast<CustomObject*>(objects[pp_index])->set_max_y(static_cast<CustomObject*>(objects[pp_index])->get_min_y());
		// adjust for blocksize
		static_cast<CustomObject*>(objects[pp_index])->set_max_y(static_cast<CustomObject*>(objects[pp_index])->get_max_y() - (static_cast<CustomObject*>(objects[pp_index])->get_max_y() % blocksize));
		if(old_height != static_cast<CustomObject*>(objects[pp_index])->get_max_y())
			recursive_bump(pp_index, static_cast<CustomObject*>(objects[pp_index])->get_max_y() - old_height);
   }

	// see if mouse_pos passes blocksize thresh hold
	if(abs(static_cast<CustomObject*>(objects[pp_index])->distance2Face(pp_face, mouse_pos)) > blocksize)
	{
		// IF PUSH
		if(static_cast<CustomObject*>(objects[pp_index])->distance2Face(pp_face, mouse_pos) < 0)
			adjust_face(pp_index, pp_face, mouse_pos, true, false);

		// IF PULL
		else
			adjust_face(pp_index, pp_face, mouse_pos, true, true);
	}
}
