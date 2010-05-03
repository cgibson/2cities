#ifndef _INGAMESTATE_H_
#define _INGAMESTATE_H_

#include <stdlib.h>

#include "../system/enum.h"
using namespace enumeration;

class InGameState
{
    public:
       InGameState();
       virtual ~InGameState();

       virtual void initialize();
       virtual void update(long milli_time);
       virtual void updateInput(long milli_time);

       virtual enum E_STATE stateType() { return GLOBAL_STATE; }

    protected:
       double theta;
       double phi;
       double distance;
};

#endif
