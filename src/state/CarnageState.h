/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include <stdlib.h>

#include "InGameState.h"
#include "../physics/Physics.h"
#include "../system/enum.h"

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
