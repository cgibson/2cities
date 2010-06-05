#include "StateManager.h"
#ifdef CLIENT
	#include "../graphics/graphics.h"
	#include "../graphics/hud.h"
#endif

#include "BuildState.h"
#include "CarnageState.h"
#include "MenuState.h"
#include "../system/global.h"
#include "../system/enum.h"

using namespace enumeration;

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
	//global::networkManager->network->emptyWorld();
    switch(newState)
    {
    case MENU_STATE:
		currentState = new MenuState();
		currentState->initialize();
#ifdef CLIENT
        gfx::hud.swapUI(Hud::MENU);
        //TODO: Add Menu State creation, initialization here
        global::soundManager->stopPlayingMusic();
        global::soundManager->playMenuSong();
#endif
        break;
    case BUILD_STATE:
        currentState = new BuildState();
        currentState->initialize();
#ifdef CLIENT
        gfx::hud.swapUI(Hud::BUILD);
        global::soundManager->stopPlayingMusic();
        global::soundManager->playBuildSong();
#endif
		currentState->objects.empty();
        break;
    case CARNAGE_STATE:
		currentState = new CarnageState();
		currentState->initialize();
#ifdef CLIENT
		gfx::hud.swapUI(Hud::CARNAGE);
		global::soundManager->stopPlayingMusic();
        global::soundManager->playCarnageSong();
#endif
		currentState->objects.empty();
        break;
    case RESULTS_STATE:
		currentState->setRealStateType(enumeration::RESULTS_STATE);
#ifdef CLIENT
		gfx::hud.swapUI(Hud::RESULTS);
		global::soundManager->stopPlayingMusic();
		global::soundManager->playResultWinSong();
#endif
        break;
    default:
		currentState->objects.empty();
        break;
    }
    //delete oldInGameState;
}

void StateManager::switchToCarnage() {
	if(currentState->stateType() == BUILD_STATE) {
		((BuildState *)currentState)->save_level("InGameSwitch.lvl");
		changeCurrentState(CARNAGE_STATE);
		networkManager->network->loadLevel("InGameSwitch.lvl");
	}
}

/**
 * Register console command for changing states.
 */
void StateManager::initialize()
{	
#ifdef CLIENT
	//FULL COMMANDS
	gfx::hud.console.registerCmd("changestate", StateManager::stateConsoleCmds);
	gfx::hud.console.registerCmd("ready", StateManager::stateConsoleCmds);

	//SHORTCUTS
	gfx::hud.console.registerCmd("cs", StateManager::stateConsoleCmds);
	gfx::hud.console.registerCmd("r", StateManager::stateConsoleCmds);
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
			if(!strcmp(argv[1], "menu") || !strcmp(argv[1], "m"))
			{
				gfx::hud.console.info("changing to Menu State");
				global::stateManager->changeCurrentState(enumeration::MENU_STATE);
				return;
			}
			if(!strcmp(argv[1], "carnage") || !strcmp(argv[1], "c"))
			{
				gfx::hud.console.info("changing to Carnage State");
				global::stateManager->changeCurrentState(enumeration::CARNAGE_STATE);
				return;
			}
			if(!strcmp(argv[1], "build") || !strcmp(argv[1], "b"))
			{
				gfx::hud.console.info("changing to Build State");
				global::stateManager->changeCurrentState(enumeration::BUILD_STATE);
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
	if(!strcmp(argv[0], "ready") || !strcmp(argv[0], "r")) {
		global::networkManager->network->setPlayerReady(1);
		gfx::hud.console.info("I'm Ready!");
	}
#endif
}
