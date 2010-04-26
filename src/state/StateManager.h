/**
 * Contains prototypes and definitions for the
 * GlobalGameState class.
 */

#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "InGameState.h"
#include "E_State.h"

class StateManager
{
    public:
        void* preferences;
        InGameState* currentState;
        StateManager();
        void changeCurrentState(e_state newState);
        void setPreference(int prefID, int newPref);
        int getPreference(int prefID);
};
#endif
