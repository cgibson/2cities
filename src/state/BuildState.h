#ifndef _BUILDSTATE_H_
#define _BUILDSTATE_H_

#include <stdlib.h>

#include "InGameState.h"

#include "../system/enum.h"
using namespace enumeration;

class BuildState: public InGameState
{
    public:
	   BuildState();
       virtual ~BuildState();

       void initialize();
       //void update(long milli_time);
       //void updateInput(long milli_time);
       //void updateCamera();

       virtual enum E_STATE stateType() { return BUILD_STATE; }
};

#endif
