/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include "BuildingUnit.h"
#include "AmmoUnit.h"

/**
 * These definitions are here for this milestone. Change
 * them if you want.
 */
#define NUM_BUNITS 30
#define NUM_AMMO 10


class CarnageState: public InGameState
{
    public:
        BuildingUnit bunits[NUM_BUNITS];
        AmmoUnit aunits[NUM_AMMO];
        void keyCallback();
        void mouseCallback();
};

#endif
