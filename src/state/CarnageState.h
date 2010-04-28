/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include "InGameState.h"
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

        CarnageState();
        virtual ~CarnageState();

        virtual void initialize();
//        virtual void update(int val);

    protected:
        virtual void mouse_click(int button, int state, int x, int y);
        virtual void mouse_motion(int x, int y);
};

#endif
