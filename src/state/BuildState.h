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
	extern int blocksize, rect_element, counter;
	extern Face intersection;
	extern Point firstPoint, last, mouse_click;
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
		static void mouseDownHandler(int button);
		static void mouseUpHandler(int button);

		void recursive_bump(int bottom, int delta_height);
		void blockalize_face(int index, Face f, bool pull);
		bool inPullPath(int reMax, int reMin, int iMax, int iMin);
		void check_pull(int index, Face f);
		void recursive_push(Face f, int bottom);
		void adjust_face(int index, Face f, Point mouse_pos, bool block, bool pull);
		void new_push_pull(Point mouse_pos);


       virtual enum E_STATE stateType() { return BUILD_STATE; }
};

#endif
