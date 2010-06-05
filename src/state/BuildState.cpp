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
#include "../helper/Vector.h"
#include "../system/io.h"

#ifdef CLIENT
	#include "../graphics/orbitalcamera.h"
	#include "../graphics/graphics.h"
#endif


namespace BuildStateGlobals
{
	bool DELETE_MODE;
	bool MOUSE_DOWN;
	bool VALID_CLICK; // invalid clicks include: left click anywhere but on a face, right click on anything but the ground
	int LAST_BUTTON;
	int RESOURCES;
	int blocksize, pp_index, counter;
	Face pp_face;
	Point firstPoint, secondPoint;
	
	bool renderPlane = false;
	Vector planeNormal = Vector(1,0,0);
	Vector planeLocation = Vector(0,5,0);
	float planeSize = 100;
	Point mapMax = Point(60, 0, 60);
	Point mapMin = Point(-60, 0, -60);
}

#define ANGLE_SPEED 2
#define MOUSE_SPEED 1
#define STRAFE_SPEED 20
#define PI 3.1415
#define DIST 30

using namespace io;
using namespace global;
using namespace BuildStateGlobals;

const int BuildState::MUSIC_DELAY = 1000;

BuildState::BuildState() {
   //initialize();
}

BuildState::~BuildState() {}

void BuildState::save_level(const char * fileName)
{
	InGameState *currState = global::stateManager->currentState;

	ofstream myfile;
   myfile.open(fileName);
   for(unsigned int i = 0; i < currState->objects.size(); i++)
   {
      // write rectangle's max and min to file
      myfile << (static_cast<CustomObject*>(currState->objects[i])->get_min_x()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_min_y()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_min_z()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_max_x()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_max_y()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_max_z()) << " 2" << endl;
   }
}

void BuildState::save_level(int argc, char *argv[])
{
	((BuildState *)global::stateManager->currentState)->save_level(argv[1]);
}

void BuildState::initialize() {
#ifdef CLIENT
	gfx::hud.console.registerCmd("save_level", BuildState::save_level);
	music_delay = 0;
	realStateType = enumeration::BUILD_STATE;
#endif

	DELETE_MODE = false;
	MOUSE_DOWN = false;
	counter = 0;
	blocksize = 5;
	pp_index = -1;
	pp_face = NOTHING;
	firstPoint = Point();

#ifdef CLIENT
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
	//io::register_key_down(BuildState::keyDownHandler);
#endif
	// make a simple CustomObject and add it to static_cast<CustomObject*>(objects vector
	// (unsigned int newid, unsigned int newplayerid, ObjectType newtype, Point newmax, Point newmin)
	//CustomObject co = new CustomObject(0, 0, CUSTOM_BLOCK, Point(25, 0, 25), Point(-25, 25, -25));
	//if(objects.size() == 0)
		//objects.push_back(new CustomObject(0, 0, CUSTOM_BLOCK, Point(5, 10, 5), Point(-5, 0, -5)));
	//printf("BUILD STATE INITIALIZE\n objects.size() = %d\n", objects.size());
}

void BuildState::update(long milli_time) {
	music_delay -= milli_time;
	music_delay = (music_delay < 0) ? 0 : music_delay; // clamp to 0
   updateInput(milli_time);
}

void BuildState::updateInput(long milli_time) {
   // General Mouse Layout
	if(MOUSE_DOWN)
		mouseDownHandler();

	// General Keyboard Layout
	if(io::keys['r']) {
	  io::keys['r'] = false;
	  networkManager->network->setPlayerReady(1);
	}

	if(io::keys[27]) {
		networkManager->network->serverDisconnect();
		exit(0);
	}
#ifdef CLIENT
	//stop music
	if(io::keys['['])
	{
		printf("Stopping the build state music.\n");
		global::soundManager->stopPlayingMusic();
	}
	
	//play music for carnage state
	if(io::keys[']'] && music_delay <= 0)
	{
		printf("Playing build state music\n");
		global::soundManager->playBuildSong();
		music_delay = MUSIC_DELAY;
	}
	
	if(io::keys['n'] && music_delay <= 0)
	{
		printf("Playing the next build state song\n");
		global::soundManager->playNextBuildSong();
		music_delay = MUSIC_DELAY;
	}
#endif
}

bool BuildState::isValidClick(Point click, int button)
{
	// check if firstPoint intersects with any existing rectangles	
	evaluateClick(click);	

	// right mouse valid clicks are on the ground only	
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(!isOutsideMap(click) && click.gety() < 1)
		{
			VALID_CLICK = true;
			return true;
		}
	}
	// left mouse valid clicks are on other rectangles only
	else if(button == GLUT_LEFT_BUTTON)
	{
		if(pp_face != NOTHING)
		{
			VALID_CLICK = true;
			return true;
		}
	}
	VALID_CLICK = false;
	return false;
}

// called only on the first click down
void BuildState::mouseDownToggle(int button)
{
	LAST_BUTTON = button;
	MOUSE_DOWN = true;
	firstPoint.set(Point(io::mouse_x, io::mouse_y));
	firstPoint.round();
}

// called only on the first mouse up
void BuildState::mouseUpToggle(int button)
{
	//InGameState *currState = global::stateManager->currentState;
	//if(button == GLUT_RIGHT_BUTTON)
		//static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
	LAST_BUTTON = button;
	MOUSE_DOWN = false;
	counter = 0;
	pp_face = NOTHING;
	pp_index = -1;
	renderPlane = false;
}

void BuildState::mouseDownHandler()
{
	InGameState *currState = global::stateManager->currentState;

	// IF firstPoint hasn't been evaluated yet
	if(counter == 0)
		isValidClick(firstPoint, LAST_BUTTON);

	// IF right click
	if(VALID_CLICK && LAST_BUTTON == GLUT_RIGHT_BUTTON)
	{
		// IF first click
		if(counter == 0)
		{
			// 1. secondPoint = fp
			secondPoint = firstPoint;
			// 2. check if click passes a blocksize threshhold
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.round();
			// check x
			// IF the click is at least blocksize distance from the firstPoint 	
			if( fabs(click.getx() - secondPoint.getx()) > blocksize )
			{
				// IF the click has a greater x value that the first point				
				if(click.getx() > secondPoint.getx())
					// increment second point by blocksize
					secondPoint.setx(secondPoint.getx() + blocksize);
				else
					secondPoint.setx(secondPoint.getx() - blocksize);
			}
			//check z
			if( fabs(click.getz() - secondPoint.getz()) > blocksize )
			{
				if(click.getz() > secondPoint.getz())
					secondPoint.setz(secondPoint.getz() + blocksize);
				else
					secondPoint.setz(secondPoint.getz() - blocksize);
			}
			// 3. create object
			int objID = currState->objects.size();
			currState->objects.push_back(new CustomObject(objID, 0, CUSTOM_BLOCK, firstPoint, secondPoint ) );
			placeY(objID, pp_index);
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->orientRect(firstPoint, secondPoint);
		}
		// IF not first click
		else
		{
			// 1. check if click passes a blocksize threshhold
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.round();
			if(!isOutsideMap(click))
			{
				// check x
				// IF the new location of the mouse and the old location of the second point have a greater x distance between them than blocksize
				if( fabs(click.getx() - secondPoint.getx()) > blocksize )
				{
					// IF the click is greater than theold location of the second point				
					if(click.getx() > secondPoint.getx())
						secondPoint.setx(secondPoint.getx() + blocksize);
					else
						secondPoint.setx(secondPoint.getx() - blocksize);
				}
				//check z
				if( fabs(click.getz() - secondPoint.getz()) > blocksize )
				{
					if(click.getz() > secondPoint.getz())
						secondPoint.setz(secondPoint.getz() + blocksize);
					else
						secondPoint.setz(secondPoint.getz() - blocksize);
				}
			
				// 2. update object (send fp and secondPoint)
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(), Point());
				co.orientRect(firstPoint, secondPoint);
				if(!isInsideRect(co, objects.size() - 1))
					static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->orientRect(firstPoint, secondPoint);
			}
		}
	}
	// IF left click                 from src/state/BuildState.cpp:17:
	else if(VALID_CLICK && LAST_BUTTON == GLUT_LEFT_BUTTON)
	{
		// IF mouse has been left clicked on an existing rectangle
		if(pp_index != -1)
		{
			// get push/pull plane
			get_pp_plane(pp_face);			
			new_push_pull(Point(io::mouse_x, io::mouse_y));
		}
	}
	// IF right and on building >> MOVE
	else if(!VALID_CLICK && pp_index != -1 && LAST_BUTTON == GLUT_RIGHT_BUTTON)
	{
		Point lastMax = static_cast<CustomObject*>(currState->objects[pp_index])->get_max();
		Point lastMin = static_cast<CustomObject*>(currState->objects[pp_index])->get_min();

		get_pp_plane(FACE1);
		Point click;
		click.set(Point(io::mouse_x, io::mouse_y));
		click.round();
		
		// check if outside map dimensions
		Point newMax = Point(static_cast<CustomObject*>(currState->objects[pp_index])->get_max_x() + click.getx() - secondPoint.getx(), 
									static_cast<CustomObject*>(currState->objects[pp_index])->get_max_y(),
									static_cast<CustomObject*>(currState->objects[pp_index])->get_max_z() + click.getz() - secondPoint.getz());
		Point newMin = Point(static_cast<CustomObject*>(currState->objects[pp_index])->get_min_x() + click.getx() - secondPoint.getx(),
									static_cast<CustomObject*>(currState->objects[pp_index])->get_min_y(),
									static_cast<CustomObject*>(currState->objects[pp_index])->get_min_z() + click.getz() - secondPoint.getz());
		CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, newMax, newMin);
		if(!(isOutsideMap(newMax) || isOutsideMap(newMin)))
		{
			if(counter > 2 && !isInsideRect(co, pp_index))
			{
				static_cast<CustomObject*>(currState->objects[pp_index])->set_max(newMax);
				static_cast<CustomObject*>(currState->objects[pp_index])->set_min(newMin);
			}
			secondPoint = click;
		}
	}
	counter++;
}

/*// 
bool BuildState::isOutOfResources()
{
	
}*/

// 2 things are required for co to be inside of a rectangle
bool BuildState::isInsideRect(CustomObject A, int excluded)
{
	InGameState *currState = global::stateManager->currentState;
	bool cond1, cond2, cond3, cond4;
	for(int i = 0; i < objects.size(); i++)
	{
		// exlcuding self
		if(i != excluded)
		{
			// condition 1: A's left is right of i's right
			cond1 = A.get_max_z() < static_cast<CustomObject*>(currState->objects[i])->get_min_z();
			// condition 2: A's right is left of i's left
			cond2 = A.get_min_z() > static_cast<CustomObject*>(currState->objects[i])->get_max_z();
			// condition 3: A's top is below i's bottum		
			cond3 = A.get_min_x() > static_cast<CustomObject*>(currState->objects[i])->get_max_x();
			// condition 4: A's bottom is above i's top
			cond4 = A.get_max_x() < static_cast<CustomObject*>(currState->objects[i])->get_min_x();
			
			if(!cond1 && !cond2 && !cond3 && !cond4)
			{
				//printf("is inside rectangle. excluded=%d, i=%d\n", excluded, i);				
				return true; 
			}
		}
	}
	return false;
}

bool BuildState::isOutsideMap(Point p)
{
	// IF player 0 (positive x) (blue)
	if(global::networkManager->network->getMyPlayerID() == 2)
	{	
		if(p.getx() > (global::map_width / 2.0) || p.getx() < 5)
			return true;
		if(p.getz() > (global::map_height / 2.0) || p.getz() < -(global::map_height / 2))
			return true;
	}
	// IF player 1 (negative x) (red)
	else
	{
		if(p.getx() < -(global::map_width / 2.0) || p.getx() > -5)
			return true;
		if(p.getz() > (global::map_height / 2.0) || p.getz() < -(global::map_height / 2))
			return true;
	}
	return false;
}

// given the current location of the mouse and the pp_face 
// determines the plane that gluUnproject samples against
void BuildState::get_pp_plane(Face f)
{
#ifdef CLIENT
	// Vertical	pushPull
	if(f == TOP)
	{
		renderPlane = true;
		planeNormal = global::camera->viewVec() * -1;
		Vector planeLoc = Vector(firstPoint.getx(), firstPoint.gety(), firstPoint.getz());
		planeLocation = planeLoc;
		planeSize = 100;
	}
	// Horizontal pushPull
	else if(f != NOTHING)
	{
		renderPlane = true;
		Vector v = Vector(0,1,0);
		planeNormal = 	v;
		v = Vector(firstPoint.getx(), firstPoint.gety(), firstPoint.getz());
		planeLocation = v;
		//printf("v: %s\n", planeLocation.str());
		planeSize = 100;
	}
#endif
}

// determines if a mouse click intersects with an existing rectangle
// if it does, it sets global variables pp_index (the index of the rectangle it intersects with)
// and pp_face (the face of the rectangle it intersects with)
void BuildState::evaluateClick(Point click)
{
	InGameState *currState = global::stateManager->currentState;
	unsigned int i = 0;	

	// FOR all objects
	while (i < currState->objects.size() && pp_face == NOTHING)
	{
		pp_face = static_cast<CustomObject*>(currState->objects[i])->check_click(click);
		if(pp_face != NOTHING)
		{
			pp_index = i;
			/* check if click is on edge
			if(click.getx() == static_cast<CustomObject*>(currState->objects[pp_index])->get_max_x() && 
				click.getz() == static_cast<CustomObject*>(currState->objects[pp_index])->get_max_z())
			{
				firstPoint.setz(firstPoint.getz() - 1);
			}
			else if(click.getx() == static_cast<CustomObject*>(currState->objects[pp_index])->get_max_x() && 
				click.getz() == static_cast<CustomObject*>(currState->objects[pp_index])->get_min_z())
			{
				firstPoint.setx(firstPoint.getx() - 1);
			}
			else if(click.getx() == static_cast<CustomObject*>(currState->objects[pp_index])->get_min_x() && 
				click.getz() == static_cast<CustomObject*>(currState->objects[pp_index])->get_min_z())
			{
				firstPoint.setz(firstPoint.getz() + 1);
			}
			else if(click.getx() == static_cast<CustomObject*>(currState->objects[pp_index])->get_min_x() && 
				click.getz() == static_cast<CustomObject*>(currState->objects[pp_index])->get_max_z())
			{
				firstPoint.setx(firstPoint.getx() + 1);
			}*/
		}
		i++;
	}
	
	/*if(pp_face == NOTHING)
		printf("clicked nothing, currState->objects.size() = %d\n", currState->objects.size());
	else
		printf("clicked rectangle %d's face %d\n", pp_index, pp_face);*/
}

void BuildState::placeY(int rect_index, int below_index)
{
	/*// IF rect_index is on top of another rectangle
	if(below_index != -1)
	{
		static_cast<CustomObject*>(objects[rect_index])->set_min_y(static_cast<CustomObject*>(objects[rect_index])->get_max_y());
		static_cast<CustomObject*>(objects[rect_index])->set_max_y(static_cast<CustomObject*>(objects[rect_index])->get_max_y());
	}
	// IF rect_index is on the ground plane
	else
	{*/
		static_cast<CustomObject*>(objects[rect_index])->set_min_y(0);
		static_cast<CustomObject*>(objects[rect_index])->set_max_y(0);
	//}
}

void BuildState::adjust_face(int index, Face f, Point click, bool pull)
{
	if(f == FACE1)
	{
		// 1. check if click passes a blocksize threshhold
		// IF the new location of the mouse and the old location of the second point have a greater x distance between them than blocksize
		if( fabs(click.getx() - static_cast<CustomObject*>(objects[index])->get_max_x()) > blocksize )
		{
			// IF the click is greater than the old location of the second point (PULL)				
			if(click.getx() > static_cast<CustomObject*>(objects[index])->get_max_x())
			{
				// IF new point is not outside map
				if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize, static_cast<CustomObject*>(objects[index])->get_max_y(), static_cast<CustomObject*>(objects[index])->get_max_z())))
				{					
					// IF new point is not inside another rect
					CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize, 0, static_cast<CustomObject*>(objects[index])->get_max_z()), static_cast<CustomObject*>(objects[index])->get_min());
					if(!isInsideRect(co, index))					
						static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize);
				}
			}
			// PUSH
			else
				static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() - blocksize);

			// 2. check if max < min
			// IF the max (as a result of the mouse move) is less than the min
			if(static_cast<CustomObject*>(objects[index])->get_max_x() <= static_cast<CustomObject*>(objects[index])->get_min_x())
			{
				// remove the rectangle
				objects.erase( objects.begin() + index);
				// IF this rectangle is the one that was initially selected from mouse() -- bookkeeping
				if(index == pp_index)
				{
					// restore initial state
					pp_index = -1;
		      	pp_face = NOTHING;
				}
			}
		}
	}
	else if(f == FACE2)
	{
		if( fabs(click.getz() - static_cast<CustomObject*>(objects[index])->get_max_z()) > blocksize )
		{
			if(click.getz() > static_cast<CustomObject*>(objects[index])->get_max_z())
			{
				if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_max_x(), static_cast<CustomObject*>(objects[index])->get_max_y(), static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize)))	
				{					
					// IF new point is not inside another rect
					CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x(), 0, static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize), static_cast<CustomObject*>(objects[index])->get_min());
					if(!isInsideRect(co, index))					
						static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize);
				}				
			}
			else
				static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() - blocksize);

			if(static_cast<CustomObject*>(objects[index])->get_max_z() <= static_cast<CustomObject*>(objects[index])->get_min_z())
			{
				objects.erase( objects.begin() + index);

				if(index == pp_index)
				{
					pp_index = -1;
		      	pp_face = NOTHING;
				}
			}
		}
	}
	else if(f == FACE3)
	{
		if( fabs(click.getx() - static_cast<CustomObject*>(objects[index])->get_min_x()) > blocksize )
		{
			if(click.getx() > static_cast<CustomObject*>(objects[index])->get_min_x())
				static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() + blocksize);
			else if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize, static_cast<CustomObject*>(objects[index])->get_min_y(), static_cast<CustomObject*>(objects[index])->get_min_z())))
			{					
				// IF new point is not inside another rect
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, static_cast<CustomObject*>(objects[index])->get_max(), Point(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize, 0, static_cast<CustomObject*>(objects[index])->get_min_z()));
				if(!isInsideRect(co, index))					
					static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize);
			}

			if(static_cast<CustomObject*>(objects[index])->get_min_x() >= static_cast<CustomObject*>(objects[index])->get_max_x())
			{
				objects.erase( objects.begin() + index);

				if(index == pp_index)
				{
					pp_index = -1;
		      	pp_face = NOTHING;
				}
			}
		}
	}
	else if(f == FACE4)
	{
		if( fabs(click.getz() - static_cast<CustomObject*>(objects[index])->get_min_z()) > blocksize )
		{
			if(click.getz() > static_cast<CustomObject*>(objects[index])->get_min_z())
				static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() + blocksize);
			else if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_min_x(), static_cast<CustomObject*>(objects[index])->get_min_y(), static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize)))
			{					
				// IF new point is not inside another rect
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, static_cast<CustomObject*>(objects[index])->get_max(), Point(static_cast<CustomObject*>(objects[index])->get_min_x(), 0, static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize));
				if(!isInsideRect(co, index))					
					static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize);
			}			

			if(static_cast<CustomObject*>(objects[index])->get_min_z() >= static_cast<CustomObject*>(objects[index])->get_max_z())
			{
				objects.erase( objects.begin() + index);

				if(index == pp_index)
				{
					pp_index = -1;
		      	pp_face = NOTHING;
				}
			}
		}
	}
}

void BuildState::new_push_pull(Point mouse_pos)
{
   if(pp_face == TOP)
   {
      // move static_cast<CustomObject*>(objects on top of this rectangle too
      // record old height
      //int old_height = static_cast<CustomObject*>(objects[pp_index])->get_max_y();
      // update height
      static_cast<CustomObject*>(objects[pp_index])->set_max_y(mouse_pos.gety());
		// get_max_y() cannot be < get_min_y()
      if(static_cast<CustomObject*>(objects[pp_index])->get_max_y() < static_cast<CustomObject*>(objects[pp_index])->get_min_y())
         static_cast<CustomObject*>(objects[pp_index])->set_max_y(static_cast<CustomObject*>(objects[pp_index])->get_min_y());
		// adjust for blocksize
		static_cast<CustomObject*>(objects[pp_index])->set_max_y(static_cast<CustomObject*>(objects[pp_index])->get_max_y() - (static_cast<CustomObject*>(objects[pp_index])->get_max_y() % blocksize));
		//if(old_height != static_cast<CustomObject*>(objects[pp_index])->get_max_y())
			//recursive_bump(pp_index, static_cast<CustomObject*>(objects[pp_index])->get_max_y() - old_height);
   }
	
	else
	{
		mouse_pos.round();
		// IF PUSH
		if(static_cast<CustomObject*>(objects[pp_index])->distance2Face(pp_face, mouse_pos) < 0)
			adjust_face(pp_index, pp_face, mouse_pos, false);

		// IF PULL
		else
			adjust_face(pp_index, pp_face, mouse_pos, true);
	}
}
