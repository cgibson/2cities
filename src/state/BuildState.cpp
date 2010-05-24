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
	int blocksize, rect_element, counter;
	Face intersection;
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
	blocksize = 10;
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
	objects.push_back(new CustomObject(0, 0, CUSTOM_BLOCK, Point(25, 0, 25), Point(-25, 25, -25)));
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
	printf("first click io::mouse_x %d io::mouse_y %d\n", io::mouse_x, io::mouse_y);
	LAST_BUTTON = button;
	MOUSE_DOWN = true;
	firstPoint.set(Point(io::mouse_x, io::mouse_y));
	printf("firstPoint.x %f firstPoint.y %f firstPoint.z %f\n", firstPoint.getx(), firstPoint.gety(), firstPoint.getz());
	firstPoint.adjustPointForBlocksize(blocksize);
	printf("firstPoint.x %f firstPoint.y %f firstPoint.z %f\n", firstPoint.getx(), firstPoint.gety(), firstPoint.getz());
}

// called only on the first mouse up
void BuildState::mouseUpToggle(int button)
{
	InGameState *currState = global::stateManager->currentState;
	printf("last click\n");
	static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
	LAST_BUTTON = button;
	MOUSE_DOWN = false;
	counter = 0;
}

void BuildState::mouseDownHandler()
{
	InGameState *currState = global::stateManager->currentState;

	// IF frist click
	if(counter == 0)
	{
		printf("second point (drag)\n");
		// draw 2D rectangle ground surface or top of existing building
		if(LAST_BUTTON == GLUT_RIGHT_BUTTON)
		{
			// record point
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.adjustPointForBlocksize(blocksize);
			// push_back new CustomObject
			currState->objects.push_back( new CustomObject( currState->objects.size(), 0, CUSTOM_BLOCK, firstPoint, Point(io::mouse_x, io::mouse_y) ) );
			placeY(currState->objects.size() - 1, -1);
			printf("added rectangle\n");
		}
		// push / pull rectangles
		else if(LAST_BUTTON == GLUT_LEFT_BUTTON)
		{

		}
		counter++;
	}
	// ELSE not the first click
	else
	{
		if(LAST_BUTTON == GLUT_RIGHT_BUTTON)
		{
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.adjustPointForBlocksize(blocksize);
			printf("counter %d\n", counter);
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_min(firstPoint);
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_x(click.getx());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_z(click.getz());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_y(static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->get_min_y());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->adjust_bases();
			placeY(currState->objects.size() - 1, -1);
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
			counter++;
		}
		else if(LAST_BUTTON == GLUT_LEFT_BUTTON)
		{

		}
	}
}

/*void BuildState::mouseUpHandler(int button)
{
	InGameState *currState = global::stateManager->currentState;
	// IF frist registered up (after a click)
	if(MOUSE_DOWN)
	{
		// draw 2D rectangle ground surface or top of existing building
		if(button == GLUT_RIGHT_BUTTON)
		{
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.adjustPointForBlocksize(blocksize);

			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_min(firstPoint);
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_x(click.getx());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_z(click.getz());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->set_max_y(static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->get_min_y());
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->adjust_bases();

			firstPoint.setx(-1);
			firstPoint.sety(-1);
			counter = 0;

			if(currState->objects.size() != 0)
				static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
		}
		// push / pull rectangles
		else if(button == GLUT_LEFT_BUTTON)
		{
			if(rect_element != -1)
				static_cast<CustomObject*>(currState->objects[rect_element])->print_rectangle();
			rect_element = -1;
         intersection = NOTHING;
		}
		MOUSE_DOWN = false;
	}
	// ELSE handle camera movement
	else
	{

	}
}*/

void BuildState::placeY(int rect_index, int below_index)
{
	static_cast<CustomObject*>(objects[rect_index])->set_min_y(0);
	static_cast<CustomObject*>(objects[rect_index])->set_max_y(0);
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
         // IF i.get_min_y() == rect_element.get_max_y() (old_height)
         if( static_cast<CustomObject*>(objects[top])->set_min_y( (static_cast<CustomObject*>(objects[bottom])->get_max_y() - delta_height) ) )
         {
            // IF rect_element encapsulates i
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

// re is the rect_element(the rect i'm moving), i is the rectangle being compared to re
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
				// IF i is "in front of" rect_element AND rect_element "passes" another rectangle
	   		if( static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_min_x() && static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_min_x() )
		   	{
					// check if i is not on the same "level" as index
					if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					{
						// check if i is in the path of rect_element
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
			// IF i.get_min_y() == rect_element.get_max_y()
         if( static_cast<CustomObject*>(objects[top])->get_min_y() == static_cast<CustomObject*>(objects[bottom])->get_max_y() )
         {
				// IF rect_element encapsulates i ON THREE SIDES
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
			if(index == rect_element)
			{
				// restore initial state
				rect_element = -1;
         	intersection = NOTHING;
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
			if(index == rect_element)
			{
				rect_element = -1;
         	intersection = NOTHING;
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
			if(index == rect_element)
			{
				rect_element = -1;
         	intersection = NOTHING;
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
			if(index == rect_element)
			{
				rect_element = -1;
         	intersection = NOTHING;
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
	if(intersection == TOP)
   {
      // move static_cast<CustomObject*>(objects on top of this rectangle too
      // record old height
      int old_height = static_cast<CustomObject*>(objects[rect_element])->get_max_y();
      // update height
      static_cast<CustomObject*>(objects[rect_element])->set_max_y(mouse_pos.gety());
		// get_max_y() cannot be < get_min_y()
      if(static_cast<CustomObject*>(objects[rect_element])->get_max_y() < static_cast<CustomObject*>(objects[rect_element])->get_min_y())
         static_cast<CustomObject*>(objects[rect_element])->set_max_y(static_cast<CustomObject*>(objects[rect_element])->get_min_y());
		// adjust for blocksize
		static_cast<CustomObject*>(objects[rect_element])->set_max_y(static_cast<CustomObject*>(objects[rect_element])->get_max_y() - (static_cast<CustomObject*>(objects[rect_element])->get_max_y() % blocksize));
		if(old_height != static_cast<CustomObject*>(objects[rect_element])->get_max_y())
			recursive_bump(rect_element, static_cast<CustomObject*>(objects[rect_element])->get_max_y() - old_height);
   }

	// see if mouse_pos passes blocksize thresh hold
	if(abs(static_cast<CustomObject*>(objects[rect_element])->distance2Face(intersection, mouse_pos)) > blocksize)
	{
		// IF PUSH
		if(static_cast<CustomObject*>(objects[rect_element])->distance2Face(intersection, mouse_pos) < 0)
			adjust_face(rect_element, intersection, mouse_pos, true, false);

		// IF PULL
		else
			adjust_face(rect_element, intersection, mouse_pos, true, true);
	}
}
