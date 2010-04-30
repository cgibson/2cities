#ifndef _BUILDSTATE_H_
#define _BUILDSTATE_H_

#include <stdlib.h>

#include "E_State.h"

class BuildState
{
    public:
	   BuildState();
       virtual ~BuildState();

       void initialize();
       //void update(long milli_time);
       //void updateInput(long milli_time);
       //void updateCamera();

       virtual enum e_state stateType() { return BUILD_S; }
};

#endif
