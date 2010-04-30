#ifndef _INGAMESTATE_H_
#define _INGAMESTATE_H_

#include <stdlib.h>

#include "E_State.h"

class InGameState
{
    public:
       InGameState();
       virtual ~InGameState();

       virtual void initialize();
       virtual void update(long milli_time);
       virtual void updateInput(long milli_time);
       virtual void updateCamera();

       virtual enum e_state stateType() { return GLOBAL_S; }

    protected:
       double theta;
       double phi;
       double distance;
       double ammo_recharge;

};

#endif
