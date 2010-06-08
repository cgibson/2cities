/**
 * Represents the Carnage State in each Two Cities game.
 */

#ifndef _CARNAGE_STATE_H_
#define _CARNAGE_STATE_H_

#include <stdlib.h>

#include "InGameState.h"
#include "../physics/Physics.h"
#include "../system/enum.h"
#include "../helper/Vector.h"
#include "../graphics/Material.h"

#include "../scene/ComplexObject.h"

using namespace std;

/**
 * These definitions are here for this milestone. Change
 * them if you want.
 */
#define NUM_BUNITS 30
#define NUM_AMMO 10

#define AMMO_COUNT 7

class CarnageState: public InGameState
{
    public:
        CarnageState();
        virtual ~CarnageState();

        void initialize();
        void update(long milli_time);
        void updateInput(long milli_time);
        void setOppPos(const Vector& newOppPos) {oppPos = newOppPos;}
        void setOppView(const Vector& newOppView) {oppView = newOppView;}
        Vector getOppPos() {return oppPos;}
        Vector getOppView() {return oppView;}
        int getRechargePercent(int ammoIndex) const {
        	return (ammoDelayTimers[ammoIndex] - ammoTimers[ammoIndex]) * 100 / ammoDelayTimers[ammoIndex];
        }
        int getAmmoCount(int ammoIndex) const { return ammoCounts[ammoIndex]; }
        void godMode() {
        	printf("God Mode!!!\n");
        	for(int i=0; i<ammoCount; i++)
        		ammoCounts[i] = 9999;
        }

        virtual enum E_STATE stateType() {return realStateType;}

		static const int MUSIC_DELAY;

        //Opponents' camera position and view vectors.
        Vector oppPos;
        Vector oppView;

        WorldObject *opponent;
        WorldObject *opponentCannon;
        WorldObject *playerCannon;

        Material *opponentMat;
        Material *playerMat;

        int ammoIndex;

    protected:
		ObjectType ammoTypes[AMMO_COUNT];
		int ammoNextAward[AMMO_COUNT];
		int ammoTimers[AMMO_COUNT];
		int ammoCounts[AMMO_COUNT];

        int ammoDelayTimers[AMMO_COUNT];
        int ammoDelayNextAward[AMMO_COUNT];

        int ammoCount;

	private:
		bool cameraSetupComplete;
};

#endif
