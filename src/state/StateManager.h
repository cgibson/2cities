/**
 * Contains prototypes and definitions for the
 * GlobalGameState class.
 */

#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#include "InGameState.h"
#include "BuildState.h"
#include "CarnageState.h"

#include "Preferences.h"
#include "PhysObj.h"			// TODO to be removed

#include "../system/enum.h"
using namespace enumeration;

class StateManager
{
    public:
        void* preferences;
        InGameState* currentState;
        StateManager();
        void changeCurrentState(enum E_STATE newState);
        void setPreference(int prefID, int newPref);
        int getPreference(int prefID);
};
#endif
