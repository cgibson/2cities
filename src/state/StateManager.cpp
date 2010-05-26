#include "StateManager.h"
#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

#include "BuildState.h"
#include "CarnageState.h"
#include "../system/global.h"

using namespace enumeration;

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
    //InGameState *oldInGameState = global::stateManager->currentState;
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
    //delete oldInGameState;
}

/**
 * Register console command for changing states.
 */
void StateManager::initialize()
{
#ifdef CLIENT
	//FULL COMMANDS
	gfx::hud.console.registerCmd("changestate", StateManager::stateConsoleCmds);

	//SHORTCUTS
	gfx::hud.console.registerCmd("cs", StateManager::stateConsoleCmds);
#endif
}

/**
 * StateManager console command handler.
 */
void StateManager::stateConsoleCmds(int argc, char *argv[])
{
#ifdef CLIENT
	if(!strcmp(argv[0], "changestate") || !strcmp(argv[0], "cs"))
	{
		if(argc == 2)
		{
			if(!strcmp(argv[1], "carnage") || !strcmp(argv[1], "c"))
			{
				gfx::hud.console.info("changing to Carnage State");
				global::stateManager->changeCurrentState(enumeration::CARNAGE_STATE);
				global::stateManager->currentState->initialize();
				return;
			}
			if(!strcmp(argv[1], "build") || !strcmp(argv[1], "b"))
			{
				gfx::hud.console.info("changing to Build State");
				global::stateManager->changeCurrentState(enumeration::BUILD_STATE);
				global::stateManager->currentState->initialize();
				return;
			}

			//no legitimate command
			gfx::hud.console.error("Usage: %s <state>", argv[0]);
			return;
		}
		else
		{
			gfx::hud.console.error("Usage: %s <state>", argv[0]);
			return;
		}
	}
#endif
}
