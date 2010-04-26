#include <stdio.h>
#include <stdlib.h>

#include "GlobalGameState.h"
#include "E_State.h"
#include "Preferences.h"
#include "CarnageState.h"
#include "PhysObj.h"

/**
 * Represents the global game state.
 *
 * @author Kennedy Owen
 * @version 20 April 2010
 */

/**
 * Initialize the current state to an (assumed) new CarnageState.
 */
GlobalGameState::GlobalGameState()
{
    preferences = (void*)malloc(sizeof(void*) * NUM_PREFERENCES);
}

/**
 * Changes the current in-game state and reregisters the GLUT
 * keyboard and mouse function registration to the newer state.
 * 
 * @param newState the newer state
 */
void GlobalGameState::changeCurrentState(e_state newState)
{
    switch(newState)
    {
    case CARNAGE_S:
        //currentState = new CarnageState();
        break;

    default:
        break;
    }
}
