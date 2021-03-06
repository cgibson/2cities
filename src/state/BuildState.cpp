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
	#include "../graphics/BlackholeEffect.h"
#endif


namespace BuildStateGlobals
{
	bool DELETE_MODE;
	bool MOUSE_DOWN;
	bool VALID_CLICK;
	int LAST_BUTTON;
	int INITIAL_RESOURCES = 75000;
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
	cameraSetupComplete = false;
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
	currResources = INITIAL_RESOURCES;
	currBuildingType = TESS_STONEHENGE;
	currBuildingType = TESS_STONEHENGE;
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
	global::camera->init(Vector(0.0, 30.0, 30.0), Vector(0.0, 5.0, 0.0));
	OrbitalCamera *cam = static_cast<OrbitalCamera *>(global::camera);
	cam->shiftCaptureMode(true);

	// mouse should be free to click
	io::release_mouse();

	io::register_mouse_down(BuildState::mouseDownToggle);
	io::register_mouse_up(BuildState::mouseUpToggle);
	//io::register_key_down(BuildState::keyDownHandler);
	updateResources();
#endif
}

void BuildState::update(long milli_time) {
	music_delay -= milli_time;
	music_delay = (music_delay < 0) ? 0 : music_delay; // clamp to 0

	// keep the camera centered on the proper half of the playing field
#ifdef CLIENT
	int playerID = global::networkManager->network->getMyPlayerID();
	if (!cameraSetupComplete && (playerID == 1 || playerID == 2))
	{
		OrbitalCamera *cam = static_cast<OrbitalCamera *>(global::camera);
		if (playerID == 1) // red (negative x)
		{
			cam->center(Vector(-global::map_width / 4.0, 5.0, 0.0));
		}
		else // blue (positive x)
		{
			cam->center(Vector(global::map_width / 4.0, 5.0, 0.0));
		}
		cameraSetupComplete = true;
	}
#endif

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
		global::soundManager->stopPlayingMusic();
	}

	//play music for carnage state
	if(io::keys[']'] && music_delay <= 0)
	{
		global::soundManager->playBuildSong();
		music_delay = MUSIC_DELAY;
	}

	if(io::keys['n'] && music_delay <= 0)
	{
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
	//secondPoint = firstPoint;
	//renderPlane = false;
}

// called only on the first mouse up
void BuildState::mouseUpToggle(int button)
{
	if(button == GLUT_RIGHT_BUTTON && VALID_CLICK == true)
	{
		if(static_cast<CustomObject*>(global::stateManager->currentState->objects[global::stateManager->currentState->objects.size() - 1])->compute_area() == 0)
			global::stateManager->currentState->objects.erase( global::stateManager->currentState->objects.begin() + 
				global::stateManager->currentState->objects.size() - 1);
	}
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
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->setBuildingType(currBuildingType);
			// set y = 0
			static_cast<CustomObject*>(objects[currState->objects.size() - 1])->set_min_y(0);
			static_cast<CustomObject*>(objects[currState->objects.size() - 1])->set_max_y(0);
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
					// IF the click is greater than the old location of the second point
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
				co.set_max_y(blocksize);
				co.setBuildingType(static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->getBuildingType());
				if(!isInsideRect(co, objects.size() - 1) && !isOutOfResources(co, objects.size() - 1))
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

void BuildState::updateResources()
{
	InGameState *currState = global::stateManager->currentState;
	//printf("BLA!!!!!\n");
	int costOfBuildings = 0;
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		costOfBuildings += static_cast<CustomObject*>(currState->objects[i])->getCost();
	}
	currResources = INITIAL_RESOURCES - costOfBuildings;
	global::networkManager->network->setMyPlayerScore(currResources);
	global::networkManager->network->setMyPlayerDamage(100 * ((INITIAL_RESOURCES - currResources) / (float)INITIAL_RESOURCES));
}

//
bool BuildState::isOutOfResources(CustomObject modifiedObject, int index)
{
	InGameState *currState = global::stateManager->currentState;

	int cost = modifiedObject.updateCost();

	// calculate increase in resources by change in CustomObject
	int difference = modifiedObject.getCost() - static_cast<CustomObject*>(currState->objects[index])->getCost();

	// IF reourcesLeft - costOfIncrease < 0
	if(currResources - difference < 0)
		return true;
	else
		return false;
}

// TRUE if min > max or max < min, set difference
bool BuildState::isPastBounds(CustomObject modifiedObject)
{
	// IF max < min
	if(modifiedObject.get_max_x() <= modifiedObject.get_min_x())
		return true;
	if(modifiedObject.get_max_z() <= modifiedObject.get_min_z())
		return true;
	// IF min > max
	if(modifiedObject.get_min_x() >= modifiedObject.get_max_x())
		return true;
	if(modifiedObject.get_min_z() >= modifiedObject.get_max_z())
		return true;

	return false;
}

// 2 things are required for co to be inside of a rectangle
bool BuildState::isInsideRect(CustomObject A, unsigned int excluded)
{
	InGameState *currState = global::stateManager->currentState;
	bool cond1, cond2, cond3, cond4;
	for(unsigned int i = 0; i < objects.size(); i++)
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
		{//printf("%s is outside map!\n", p.str());

			return true;
		}
		if(p.getz() > (global::map_height / 2.0) || p.getz() < -(global::map_height / 2))
		{//printf("%s is outside map!\n", p.str());
			return true;
		}
	}
	// IF player 1 (negative x) (red)
	else if(global::networkManager->network->getMyPlayerID() == 1)
	{
		if(p.getx() < -(global::map_width / 2.0) || p.getx() > -5)
		{//printf("%s is outside map!\n", p.str());
			return true;
		}
		if(p.getz() > (global::map_height / 2.0) || p.getz() < -(global::map_height / 2))
		{//printf("%s is outside map!\n", p.str());
			return true;
		}
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
}

bool BuildState::adjust_face(int index, Face f, Point click)
{
	if(f == TOP)
   {
		if( fabs(click.gety() - static_cast<CustomObject*>(objects[index])->get_max_y()) > blocksize )
		{
			// PULL
			if(click.gety() > static_cast<CustomObject*>(objects[index])->get_max_y())
			{
				// IF new point is not inside another rect
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x(),
																							static_cast<CustomObject*>(objects[index])->get_max_y() + blocksize,
																							static_cast<CustomObject*>(objects[index])->get_max_z()),
																								static_cast<CustomObject*>(objects[index])->get_min());
				co.setBuildingType(static_cast<CustomObject*>(objects[index])->getBuildingType());
				if(!isOutOfResources(co, index))
				{
					static_cast<CustomObject*>(objects[index])->set_max_y(static_cast<CustomObject*>(objects[index])->get_max_y() + blocksize);
				}
			}
			// PUSH
			else
			{
				static_cast<CustomObject*>(objects[index])->set_max_y(static_cast<CustomObject*>(objects[index])->get_max_y() - blocksize);
			}
			return true;
		}//printf("top\n");
   }
	else if(f == FACE1)
	{
		// 1. check if click passes a blocksize threshhold
		// IF the new location of the mouse and the old location of the second point have a greater x distance between them than blocksize
		if( fabs(click.getx() - static_cast<CustomObject*>(objects[index])->get_max_x()) > blocksize )
		{
			// IF the click is greater than the old location of the second point
			// PULL
			if(click.getx() > static_cast<CustomObject*>(objects[index])->get_max_x())
			{
				// IF new point is not outside map
				if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize, static_cast<CustomObject*>(objects[index])->get_max_y(), static_cast<CustomObject*>(objects[index])->get_max_z())))
				{
					// IF new point is not inside another rect
					CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize,
																								static_cast<CustomObject*>(objects[index])->get_max_y(),
																								static_cast<CustomObject*>(objects[index])->get_max_z()),
																									static_cast<CustomObject*>(objects[index])->get_min());
					co.setBuildingType(static_cast<CustomObject*>(objects[index])->getBuildingType());
					if(!isInsideRect(co, index) && !isOutOfResources(co, index))
					{
						static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize);
					}
				}
			}
			// PUSH
			else
			{
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x() - blocksize,
																							static_cast<CustomObject*>(objects[index])->get_max_y(),
																							static_cast<CustomObject*>(objects[index])->get_max_z()),
																									static_cast<CustomObject*>(objects[index])->get_min());
				if(isPastBounds(co))
				{
					//printf("DEAD\n");
					objects.erase( objects.begin() + index);

					pp_index = -1;
		      	pp_face = NOTHING;
				}
				else
				{
					static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() - blocksize);
				}
			}
			return true;
		}//printf("face1\n");
	}
	else if(f == FACE2)
	{
		if( fabs(click.getz() - static_cast<CustomObject*>(objects[index])->get_max_z()) > blocksize )
		{
			// PULL
			if(click.getz() > static_cast<CustomObject*>(objects[index])->get_max_z())
			{
				if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_max_x(), static_cast<CustomObject*>(objects[index])->get_max_y(), static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize)))
				{
					CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x(),
																								static_cast<CustomObject*>(objects[index])->get_max_y(),
																								static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize),
																									static_cast<CustomObject*>(objects[index])->get_min());
					co.setBuildingType(static_cast<CustomObject*>(objects[index])->getBuildingType());
					if(!isInsideRect(co, index) && !isOutOfResources(co, index))
					{
						static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize);
					}
				}
			}
			// PUSH
			else
			{
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, Point(static_cast<CustomObject*>(objects[index])->get_max_x(),
																							static_cast<CustomObject*>(objects[index])->get_max_y(),
																							static_cast<CustomObject*>(objects[index])->get_max_z() - blocksize),
																									static_cast<CustomObject*>(objects[index])->get_min());
				if(isPastBounds(co))
				{
					//printf("DEAD\n");
					objects.erase( objects.begin() + index);

					pp_index = -1;
		      	pp_face = NOTHING;
				}
				else
				{
					static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() - blocksize);
				}
			}
			return true;
		}//printf("face2\n");
	}
	else if(f == FACE3)
	{
		if( fabs(click.getx() - static_cast<CustomObject*>(objects[index])->get_min_x()) > blocksize )
		{
			// PULL
			if(click.getx() < static_cast<CustomObject*>(objects[index])->get_min_x())
			{
				if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize, static_cast<CustomObject*>(objects[index])->get_min_y(), static_cast<CustomObject*>(objects[index])->get_min_z())))
				{
					CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, static_cast<CustomObject*>(objects[index])->get_max(),
																						Point(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize,
																								static_cast<CustomObject*>(objects[index])->get_min_y(),
																								static_cast<CustomObject*>(objects[index])->get_min_z()));
					co.setBuildingType(static_cast<CustomObject*>(objects[index])->getBuildingType());
					if(!isInsideRect(co, index) && !isOutOfResources(co, index))
					{
						static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize);
					}
				}
			}
			// PUSH
			else
			{
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, static_cast<CustomObject*>(objects[index])->get_max(),
																					Point(static_cast<CustomObject*>(objects[index])->get_min_x() + blocksize,
																							static_cast<CustomObject*>(objects[index])->get_min_y(),
																							static_cast<CustomObject*>(objects[index])->get_min_z()));
				if(isPastBounds(co))
				{
					//printf("DEAD\n");
					objects.erase( objects.begin() + index);

					pp_index = -1;
		      	pp_face = NOTHING;
				}
				else
				{
					static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() + blocksize);
				}
			}
			return true;
		}//printf("face3\n");
	}
	else if(f == FACE4)
	{
		if( fabs(click.getz() - static_cast<CustomObject*>(objects[index])->get_min_z()) > blocksize )
		{
			// PULL
			if(click.getz() < static_cast<CustomObject*>(objects[index])->get_min_z())
			{
				if(!isOutsideMap(Point(static_cast<CustomObject*>(objects[index])->get_min_x(), static_cast<CustomObject*>(objects[index])->get_min_y(), static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize)))
				{
					CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, static_cast<CustomObject*>(objects[index])->get_max(),
																						Point(static_cast<CustomObject*>(objects[index])->get_min_x(),
																								static_cast<CustomObject*>(objects[index])->get_min_y(),
																								static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize));
					co.setBuildingType(static_cast<CustomObject*>(objects[index])->getBuildingType());
					if(!isInsideRect(co, index) && !isOutOfResources(co, index))
					{
						static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize);
					}
				}
			}
			// PUSH
			else
			{
				CustomObject co = CustomObject(0, 0, CUSTOM_BLOCK, static_cast<CustomObject*>(objects[index])->get_max(),
																					Point(static_cast<CustomObject*>(objects[index])->get_min_x(),
																							static_cast<CustomObject*>(objects[index])->get_min_y(),
																							static_cast<CustomObject*>(objects[index])->get_min_z() + blocksize));
				if(isPastBounds(co))
				{
					//printf("DEAD\n");
					objects.erase( objects.begin() + index);

					pp_index = -1;
		      	pp_face = NOTHING;
				}
				else
				{
					static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() + blocksize);
				}
			}
			return true;
		}//printf("face4\n");
	}
	return false;
}

void BuildState::new_push_pull(Point mouse_pos)
{
	mouse_pos.round();
	if(adjust_face(pp_index, pp_face, mouse_pos))
	{
		updateResources();
		//printf("currResources: %d\n", currResources);
	}
}
