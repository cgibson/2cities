#ifndef _BUILDSTATE_H_
#define _BUILDSTATE_H_

#include <stdlib.h>

#include "InGameState.h"
#include "../system/enum.h"
#include "../helper/Point.h"
#include "../scene/CustomObject.h"

using namespace enumeration;

namespace BuildStateGlobals
{
	extern bool DELETE_MODE;
	extern bool MOUSE_DOWN;
	extern int LAST_BUTTON;
	extern int blocksize, rect_element, counter;
	extern Face pp_face;
	extern int pp_index;
	extern Point firstPoint, last, mouse_click;
	
	extern bool renderPlane;
	extern Vector planeNormal;
	extern Vector planeLocation;
	extern float planeSize;
	extern Point mapMax, mapMin;
}

class BuildState : public InGameState
{
	public:
	   Tesselation currBuildingType;

	   BuildState();
       virtual ~BuildState();

       void initialize();
       void update(long milli_time);
       void updateInput(long milli_time);

		static void keyDownHandler(int key, bool special);
		static void mouseDownToggle(int button);
		static void mouseUpToggle(int button);
		static void save_level(int argc, char *argv[]);

		void save_level(const char * fileName);

		void mouseDownHandler();

		void recursive_bump(int bottom, int delta_height);
		void blockalize_face(int index, Face f, bool pull);
		bool inPullPath(int reMax, int reMin, int iMax, int iMin);
		bool check_pull(int index, Face f, bool move);
		void recursive_push(Face f, int bottom);
		void adjust_face(int index, Face f, Point mouse_pos, bool pull);
		void new_push_pull(Point mouse_pos);
		Point adjustPointForBlocksize(Point click);
		void placeY(int rect_index, int below_index);
		void evaluateClick(Point click);
		void get_pp_plane(Face f);
		bool isValidClick(Point click, int button);
		void checkRectBase(int index);
		Face whichFaceIsAdjusted(Point lastMax, Point lastMin, int index);
		bool isOutsideMap(Point p);
		bool isInsideRect(CustomObject co, int excluded);

       virtual enum E_STATE stateType() { return BUILD_STATE; }
       
       static const int MUSIC_DELAY;
       
       int music_delay;
};

#endif
