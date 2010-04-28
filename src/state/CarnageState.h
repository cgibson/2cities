/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include <stdlib.h>
#include <vector>

#include "InGameState.h"
#include "E_State.h"
#include "PhysObj.h"
#include "../physics/Physics.h"

using namespace std;

/**
 * These definitions are here for this milestone. Change
 * them if you want.
 */
#define NUM_BUNITS 30
#define NUM_AMMO 10

class CarnageState: public InGameState
{
    public:
        Physics physics;
        
        CarnageState();
        virtual ~CarnageState();

        void initialize();
//      void update(long milli_time);
//      void updateInput(long milli_time);

        virtual enum e_state stateType() { return CARNAGE_S; }

    protected:

};

#endif
