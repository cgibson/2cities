/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include <stdlib.h>
#include <vector>

#include "InGameState.h"
#include "../physics/Physics.h"
#include "../scene/WorldObject.h"
#include "../system/enum.h"

#include "PhysObj.h"				//TODO REMOVE

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
		vector<WorldObject> objects;
        Physics physics;  					// TODO Remove
        
        CarnageState();
        virtual ~CarnageState();

        void initialize();
        void update(long milli_time);
        void updateInput(long milli_time);

        virtual enum E_STATE stateType() { return CARNAGE_STATE; }

    protected:
        double ammo_recharge;
};

#endif
