#include "StateManager.h"

/**
 * Represents the global game state.
 *
 * @author Kennedy Owen
 * @version 20 April 2010
 */

/**
 * Initialize the current state to an (assumed) new CarnageState.
 */
StateManager::StateManager()
{
    preferences = (void*)malloc(sizeof(void*) * NUM_PREFERENCES);
}

/**
 * Changes the current in-game state and reregisters the GLUT
 * keyboard and mouse function registration to the newer state.
 * 
 * @param newState the newer state
 */
void StateManager::changeCurrentState(enum E_STATE newState)
{
    switch(newState)
    {
    case BUILD_STATE:
        currentState = new BuildState();
        break;
    case CARNAGE_STATE:
        currentState = new CarnageState();
        break;

    default:
        break;
    }
}
