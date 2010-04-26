/**
 * Contains prototypes and definitions for the
 * GlobalGameState class.
 */

#ifndef _GLOBAL_GAME_STATE_H_
#define _GLOBAL_GAME_STATE_H_

#include "InGameState.h"
#include "E_State.h"

class GlobalGameState
{
    public:
        void* preferences;
        InGameState* currentState;
        GlobalGameState();
        void changeCurrentState(e_state newState);
        void setPreference(int prefID, int newPref);
        int getPreference(int prefID);
};
#endif
