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

#include "../graphics/ShockwaveEffect.h"

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
        void setOppPos(const Vector& newOppPos) {oppPos = newOppPos;}
        void setOppView(const Vector& newOppView) {oppView = newOppView;}
        Vector getOppPos() {return oppPos;}
        Vector getOppView() {return oppView;}
        int getRechargeTimeLeft() const { return ammo_recharge; }

        virtual enum E_STATE stateType() {return realStateType;}

		static const int RECHARGE_TIME;
		static const int MUSIC_DELAY;

    protected:
        int ammo_recharge;
        ObjectType ammo_type;

        //Opponents' camera position and view vectors.
        Vector oppPos;
        Vector oppView;

	private:
		bool cameraSetupComplete;
};

#endif
