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
#include "../scene/CustomObject.h"
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
	int blocksize, pp_index, counter;
	Face pp_face;
	Point firstPoint, secondPoint;
	
	bool renderPlane = false;
	Vector planeNormal = Vector(1,0,0);
	Vector planeLocation = Vector(0,5,0);
	float planeSize = 100;
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
   //initialize();
}

BuildState::~BuildState() {}

void BuildState::save_level(const char * fileName)
{
	InGameState *currState = global::stateManager->currentState;

	ofstream myfile;
   myfile.open(fileName);
	myfile << "blocksize: " << blocksize << endl;
   for(unsigned int i = 0; i < currState->objects.size(); i++)
   {
      // write rectangle's max and min to file
      myfile << (static_cast<CustomObject*>(currState->objects[i])->get_min_x()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_min_y()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_min_z()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_max_x()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_max_y()) << " " << 
					 (static_cast<CustomObject*>(currState->objects[i])->get_max_z()) << endl;
   }
}

void BuildState::save_level(int argc, char *argv[])
{
	((BuildState *)global::stateManager->currentState)->save_level(argv[1]);
}

void BuildState::initialize() {
#ifdef CLIENT
	gfx::hud.console.registerCmd("save_level", BuildState::save_level);
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
	if(objects.size() == 0)
		objects.push_back(new CustomObject(0, 0, CUSTOM_BLOCK, Point(5, 10, 5), Point(-5, 0, -5)));
	//printf("BUILD STATE INITIALIZE\n objects.size() = %d\n", objects.size());
}

void BuildState::update(long milli_time) {
   updateInput(milli_time);
}

void BuildState::updateInput(long milli_time) {
   // General Mouse Layout
	if(MOUSE_DOWN)
		mouseDownHandler();

	// General Keyboard Layout
	if(io::keys['r']) {
	  io::keys['r'] = false;
	  networkManager->network->sendPlayerReady(1);
	}

	if(io::keys[27]) {
		networkManager->network->serverDisconnect();
		exit(0);
	}
}

bool BuildState::isValidClick(Point click, int button)
{
	// check if firstPoint intersects with any existing rectangles	
	evaluateClick(click);	

	// right mouse valid clicks are on the ground only	
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(click.gety() < 1 && click.getx() < 100 && click.getx() > -100 && click.getz() < 100 && click.getz() > -100)
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
	InGameState *currState = global::stateManager->currentState;
	if(button == GLUT_RIGHT_BUTTON)
		static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
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
		// IF first click--
		if(counter == 0)
		{
			// 1. secondPoint = fp
			secondPoint = firstPoint;
			// 2. check if click passes a blocksize threshhold
			Point click;
			click.set(Point(io::mouse_x, io::mouse_y));
			click.round();
			// check x			
			if( fabs(click.getx() - secondPoint.getx()) > blocksize )
			{
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
			static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->orientRect(firstPoint, secondPoint);
			
			//static_cast<CustomObject*>(currState->objects[currState->objects.size() - 1])->print_rectangle();
			// 3. check for intersection
			checkRectBase(currState->objects.size() - 1);
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
	else if(!VALID_CLICK && LAST_BUTTON == GLUT_RIGHT_BUTTON)
	{
		get_pp_plane(FACE1);		
		Point click;
		click.set(Point(io::mouse_x, io::mouse_y));
		click.round();
		//printf("MOVE RECT: %d\n", pp_index);
		if(counter > 2)
		{
			static_cast<CustomObject*>(currState->objects[pp_index])->set_max_x( static_cast<CustomObject*>(currState->objects[pp_index])->get_max_x() + click.getx() - secondPoint.getx() );
			static_cast<CustomObject*>(currState->objects[pp_index])->set_min_x( static_cast<CustomObject*>(currState->objects[pp_index])->get_min_x() + click.getx() - secondPoint.getx() );
			
			static_cast<CustomObject*>(currState->objects[pp_index])->set_max_z( static_cast<CustomObject*>(currState->objects[pp_index])->get_max_z() + click.getz() - secondPoint.getz() );
			static_cast<CustomObject*>(currState->objects[pp_index])->set_min_z( static_cast<CustomObject*>(currState->objects[pp_index])->get_min_z() + click.getz() - secondPoint.getz() );
		}
		secondPoint = click;
		static_cast<CustomObject*>(currState->objects[pp_index])->print_rectangle();
		// check for overlap
		check_pull(pp_index, FACE1, true);
		check_pull(pp_index, FACE2, true);
		check_pull(pp_index, FACE3, true);
		check_pull(pp_index, FACE4, true);
		static_cast<CustomObject*>(currState->objects[pp_index])->print_rectangle();
	}
	counter++;
}

// check for intersection for 2D rectangle base
void BuildState::checkRectBase(int index)
{
	InGameState *currState = global::stateManager->currentState;

	// determine which faces i'm moving
	Face f1, f2;
	// corner A
	if(firstPoint.getx() > secondPoint.getx() && firstPoint.getz() > secondPoint.getz())
	{
		f1 = FACE3;
		f2 = FACE4;
	}
	// corner B
	else if(firstPoint.getx() < secondPoint.getx() && firstPoint.getz() > secondPoint.getz())
	{
		f1 = FACE1;
		f2 = FACE4;
	}
	// corner C
	else if(firstPoint.getx() < secondPoint.getx() && firstPoint.getz() < secondPoint.getz())
	{
		f1 = FACE1;
		f2 = FACE2;
	}
	// corner D
	else if(firstPoint.getx() > secondPoint.getx() && firstPoint.getz() < secondPoint.getz())
	{
		f1 = FACE2;
		f2 = FACE3;
	}
	// FOR all rectangles
	for(unsigned int i = 0; i < currState->objects.size(); i++)
	{
		// excluding self
		if((int)i != index)
		{
			// check first face
			check_pull(index, f1, false);

			// check second face
			check_pull(index, f2, false);
		}
	}
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

// bump up all static_cast<CustomObject*>(objects on top of rect_index
void BuildState::recursive_bump(int bottom, int delta_height)
{
   // loop over all static_cast<CustomObject*>(objects
   unsigned int top;
   for(top = 0; top < objects.size(); top++)
   {
      // exclude self
      if((int)top != bottom)
      {
         // IF i.get_min_y() == pp_index.get_max_y() (old_height)
         if( static_cast<CustomObject*>(objects[top])->get_min_y() == (static_cast<CustomObject*>(objects[bottom])->get_max_y() - delta_height) )
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

// re is the pp_index(the rect i'm moving), i is the rectangle being compared to re
bool BuildState::inPullPath(int reMax, int reMin, int iMax, int iMin)
{
   // IF reMax OR reMin is INSIDE OF iMax AND iMin
   if((reMax < iMax && reMax > iMin) ||
      (reMin < iMax && reMin > iMin))
   {
      return true;
   }

   // IF iMax OR iMin is INSIDE OF reMax AND reMin
   if((iMax < reMax && iMax > reMin) ||
      (iMin < reMax && iMin > reMin))
   {
      return true;
   }

   return false;
}

// check for rectangle collision
// check for tops going beyond their bottoms
void BuildState::check_pull(int index, Face f, bool move)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if((int)i != index)
		{
			if(f == FACE1)
			{
				// IF i is "in front of" pp_index AND pp_index "passes" another rectangle
	   		if( static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_max_x() && static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_min_x() )
		   	{
					// check if i is not on the same "level" as index
					//if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					//{
						// check if i is in the path of pp_index
				   	if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_z(), static_cast<CustomObject*>(objects[index])->get_min_z(), static_cast<CustomObject*>(objects[i])->get_max_z(), static_cast<CustomObject*>(objects[i])->get_min_z()) )
					   {
							if(move)
							{
								int shiftVal = 0;
								shiftVal = fabs( static_cast<CustomObject*>(objects[index])->get_max_x() - static_cast<CustomObject*>(objects[i])->get_min_x());
								static_cast<CustomObject*>(objects[index])->set_max_x( static_cast<CustomObject*>(objects[index])->get_max_x() - shiftVal );
								static_cast<CustomObject*>(objects[index])->set_min_x( static_cast<CustomObject*>(objects[index])->get_min_x() - shiftVal );

								LAST_BUTTON = -1;
							}
							else
							{
								// bring face 1 in 1 blocksize
								while(static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_min_x())
   								static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() - blocksize);
							}
							printf("face1\n");	
	   				}
					//}
		   	}
				/*// IF index is on top of i
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
				}*/
			}
			else if(f == FACE2)
			{
				if( static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_max_z() && static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_min_z() )
				{
					//if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					//{
						if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_x(), static_cast<CustomObject*>(objects[index])->get_min_x(), static_cast<CustomObject*>(objects[i])->get_max_x(), static_cast<CustomObject*>(objects[i])->get_min_x()) )
						{						
							if(move)
							{
								int shiftVal = 0;
								shiftVal = fabs( static_cast<CustomObject*>(objects[index])->get_max_z() - static_cast<CustomObject*>(objects[i])->get_min_z());
								static_cast<CustomObject*>(objects[index])->set_max_z( static_cast<CustomObject*>(objects[index])->get_max_z() - shiftVal );
								static_cast<CustomObject*>(objects[index])->set_min_z( static_cast<CustomObject*>(objects[index])->get_min_z() - shiftVal );
								LAST_BUTTON = -1;
							}
							else
							{							
								while(static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_min_z())
									static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() - blocksize);
							}							
							printf("face2\n");
						}
					//}
				}
				
				/*if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE2, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						if(static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_max_z())
							static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[i])->get_max_z());
					}
				}*/
			}
			else if(f == FACE3)
			{
				if( static_cast<CustomObject*>(objects[index])->get_max_x() > static_cast<CustomObject*>(objects[i])->get_min_x() && static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_max_x() )
				{
					//if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					//{
						if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_z(), static_cast<CustomObject*>(objects[index])->get_min_z(), static_cast<CustomObject*>(objects[i])->get_max_z(), static_cast<CustomObject*>(objects[i])->get_min_z()) )
						{
							if(move)
							{
								int shiftVal = 0;
								shiftVal = fabs( static_cast<CustomObject*>(objects[index])->get_min_x() - static_cast<CustomObject*>(objects[i])->get_max_x());
								static_cast<CustomObject*>(objects[index])->set_max_x( static_cast<CustomObject*>(objects[index])->get_max_x() + shiftVal );
								static_cast<CustomObject*>(objects[index])->set_min_x( static_cast<CustomObject*>(objects[index])->get_min_x() + shiftVal );
								LAST_BUTTON = -1;
							}
							else
							{							
								while(static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_max_x())							
									static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() + blocksize);
							}							
							printf("face3\n");		
						}
					//}
				}
				
				/*if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE3, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						if(static_cast<CustomObject*>(objects[index])->get_min_x() < static_cast<CustomObject*>(objects[i])->get_min_x())
							static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[i])->get_min_x());
					}
				}*/
			}
			else if(f == FACE4)
			{
				if( static_cast<CustomObject*>(objects[index])->get_max_z() > static_cast<CustomObject*>(objects[i])->get_min_z() && static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_max_z() )
				{
					//if(static_cast<CustomObject*>(objects[i])->get_min_y() == static_cast<CustomObject*>(objects[index])->get_min_y())
					//{
						if( inPullPath(static_cast<CustomObject*>(objects[index])->get_max_x(), static_cast<CustomObject*>(objects[index])->get_min_x(), static_cast<CustomObject*>(objects[i])->get_max_x(), static_cast<CustomObject*>(objects[i])->get_min_x()) )
						{						
							if(move)
							{
								int shiftVal = 0;
								shiftVal = fabs( static_cast<CustomObject*>(objects[index])->get_min_z() - static_cast<CustomObject*>(objects[i])->get_max_z());
								static_cast<CustomObject*>(objects[index])->set_max_z( static_cast<CustomObject*>(objects[index])->get_max_z() + shiftVal );
								static_cast<CustomObject*>(objects[index])->set_min_z( static_cast<CustomObject*>(objects[index])->get_min_z() + shiftVal );
								LAST_BUTTON = -1;
							}
							else
							{							
								while(static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_max_z())							
									static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() + blocksize);
							}	
							printf("face4\n");			
						}					
					//}
				}
				
				/*if(static_cast<CustomObject*>(objects[index])->get_min_y() == static_cast<CustomObject*>(objects[i])->get_max_y())
				{
					if(static_cast<CustomObject*>(objects[i])->semiEncapsulates(FACE4, static_cast<CustomObject*>(objects[index])->get_max(), static_cast<CustomObject*>(objects[index])->get_min()))
					{
						if(static_cast<CustomObject*>(objects[index])->get_min_z() < static_cast<CustomObject*>(objects[i])->get_min_z())
							static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[i])->get_min_z());
					}
				}*/
			}
		}
	}
}

void BuildState::recursive_push(Face f, int bottom)
{
	unsigned int top;
	for(top = 0; top < objects.size(); top++)
	{
		// exclude self
      if((int)top != bottom)
      {
			// IF i.get_min_y() == pp_index.get_max_y()
         if( static_cast<CustomObject*>(objects[top])->get_min_y() == static_cast<CustomObject*>(objects[bottom])->get_max_y() )
         {
				// IF pp_index encapsulates i ON THREE SIDES
            if(static_cast<CustomObject*>(objects[bottom])->semiEncapsulates(f, (static_cast<CustomObject*>(objects[top])->get_max()), (static_cast<CustomObject*>(objects[top])->get_min())))
            {
					if(static_cast<CustomObject*>(objects[top])->distance2Face(f, static_cast<CustomObject*>(objects[bottom])->whichPoint(f)) < 0)
					{
						adjust_face(top, f, static_cast<CustomObject*>(objects[bottom])->whichPoint(f), false);
						recursive_push(f, top);
					}
				}
			}
		}
	}
}

void BuildState::adjust_face(int index, Face f, Point click, bool pull)
{
	if(f == FACE1)
	{
		// 1. check if click passes a blocksize threshhold
		// IF the new location of the mouse and the old location of the second point have a greater x distance between them than blocksize
		if( fabs(click.getx() - static_cast<CustomObject*>(objects[index])->get_max_x()) > blocksize )
		{
			// IF the click is greater than the old location of the second point				
			if(click.getx() > static_cast<CustomObject*>(objects[index])->get_max_x())
				static_cast<CustomObject*>(objects[index])->set_max_x(static_cast<CustomObject*>(objects[index])->get_max_x() + blocksize);
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
			// 3. check if pull collides 
			// IF the max is not less than the min and adjust is a pull operation
			else if(pull)
			check_pull(index, f, false);
		}
	}
	else if(f == FACE2)
	{
		if( fabs(click.getz() - static_cast<CustomObject*>(objects[index])->get_max_z()) > blocksize )
		{
			if(click.getz() > static_cast<CustomObject*>(objects[index])->get_max_z())
				static_cast<CustomObject*>(objects[index])->set_max_z(static_cast<CustomObject*>(objects[index])->get_max_z() + blocksize);
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
			else if(pull)
				check_pull(index, f, false);
		}
	}
	else if(f == FACE3)
	{
		if( fabs(click.getx() - static_cast<CustomObject*>(objects[index])->get_min_x()) > blocksize )
		{
			if(click.getx() > static_cast<CustomObject*>(objects[index])->get_min_x())
				static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() + blocksize);
			else
				static_cast<CustomObject*>(objects[index])->set_min_x(static_cast<CustomObject*>(objects[index])->get_min_x() - blocksize);

			if(static_cast<CustomObject*>(objects[index])->get_min_x() >= static_cast<CustomObject*>(objects[index])->get_max_x())
			{
				objects.erase( objects.begin() + index);

				if(index == pp_index)
				{
					pp_index = -1;
		      	pp_face = NOTHING;
				}
			}
			else if(pull)
				check_pull(index, f, false);
		}
	}
	else if(f == FACE4)
	{
		if( fabs(click.getz() - static_cast<CustomObject*>(objects[index])->get_min_z()) > blocksize )
		{
			if(click.getz() > static_cast<CustomObject*>(objects[index])->get_min_z())
				static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() + blocksize);
			else
				static_cast<CustomObject*>(objects[index])->set_min_z(static_cast<CustomObject*>(objects[index])->get_min_z() - blocksize);

			if(static_cast<CustomObject*>(objects[index])->get_min_z() >= static_cast<CustomObject*>(objects[index])->get_max_z())
			{
				objects.erase( objects.begin() + index);

				if(index == pp_index)
				{
					pp_index = -1;
		      	pp_face = NOTHING;
				}
			}
			else if(pull)
				check_pull(index, f, false);
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
