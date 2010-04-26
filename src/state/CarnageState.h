/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include "PhysObj.h"
#include <vector>

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
        vector<BuildingUnit> bunits;
        vector<AmmoUnit> aunits;
        void keyCallback();
        void mouseCallback();
};

#endif
