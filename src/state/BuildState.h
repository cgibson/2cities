#ifndef _BUILDSTATE_H_
#define _BUILDSTATE_H_

#include <stdlib.h>

#include "InGameState.h"
#include "../system/enum.h"
#include "../helper/Point.h"

using namespace enumeration;

class BuildState: public InGameState
{
	// global variables
	bool DELETE_MODE;
	bool MOUSE_DOWN;
	int blocksize;
	int rect_element;
	Face intersection;
    
	public:
	   BuildState();
       virtual ~BuildState();

       void initialize();
       void update(long milli_time);
       void updateInput(long milli_time);
		 void handleMouseInput();
		 void handleKeyInput();
       //void updateCamera();

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
