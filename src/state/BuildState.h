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
	extern int INITIAL_RESOURCES;
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
		int currResources;

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

		bool adjust_face(int index, Face f, Point mouse_pos);
		void new_push_pull(Point mouse_pos);
		Point adjustPointForBlocksize(Point click);
		void evaluateClick(Point click);
		void get_pp_plane(Face f);
		bool isValidClick(Point click, int button);
		Face whichFaceIsAdjusted(Point lastMax, Point lastMin, int index);
		bool isOutsideMap(Point p);
		bool isInsideRect(CustomObject co, unsigned int excluded);
		bool isOutOfResources(CustomObject modifiedObject, int index);
		bool isPastBounds(CustomObject modifiedObject);
		void updateResources();

		virtual enum E_STATE stateType() { return BUILD_STATE; }

		static const int MUSIC_DELAY;

		int music_delay;

private:
	bool cameraSetupComplete;
};

#endif
