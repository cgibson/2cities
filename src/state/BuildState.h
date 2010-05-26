#ifndef _BUILDSTATE_H_
#define _BUILDSTATE_H_

#include <stdlib.h>

#include "InGameState.h"
#include "../system/enum.h"
#include "../helper/Point.h"

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
}

class BuildState : public InGameState
{
	public:
	   BuildState();
       virtual ~BuildState();

       void initialize();
       void update(long milli_time);
       void updateInput(long milli_time);

		static void keyDownHandler(int key, bool special);
		static void mouseDownToggle(int button);
		static void mouseUpToggle(int button);

		void mouseDownHandler();

		void recursive_bump(int bottom, int delta_height);
		void blockalize_face(int index, Face f, bool pull);
		bool inPullPath(int reMax, int reMin, int iMax, int iMin);
		void check_pull(int index, Face f);
		void recursive_push(Face f, int bottom);
		void adjust_face(int index, Face f, Point mouse_pos, bool pull);
		void new_push_pull(Point mouse_pos);
		Point adjustPointForBlocksize(Point click);
		void placeY(int rect_index, int below_index);
		void evaluateClick(Point click);
		void get_pp_plane();
		bool isValidClick(Point click, int button);

       virtual enum E_STATE stateType() { return BUILD_STATE; }
};

#endif
