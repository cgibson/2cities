#include "hud.h"
#include "graphics.h"

Hud::Hud()
{
    _showConsole = false; // default off
    _currentUI = NULL; // none
    _carnageUI = NULL;
}

Hud::~Hud()
{
	if (_carnageUI != NULL) delete _carnageUI;
}

void Hud::init()
{
    console.init();

	// preload our UIs
	_carnageUI = new CarnageUI();
	_carnageUI->init();

    // register commands with the console
    console.registerCmd("exit", Hud::exitApp);
	console.registerCmd("quit", Hud::exitApp);
    console.registerCmd("clear", Hud::clearConsole);
    console.registerCmd("swapui", Hud::swapUIcmd);
}

void Hud::exitApp(int argc, char *argv[])
{
    exit(EXIT_SUCCESS);
}

void Hud::clearConsole(int argc, char *argv[])
{
    gfx::hud.console.clear();
}

void Hud::swapUIcmd(int argc, char *argv[])
{
    if (argc != 2)
    {
        gfx::hud.console.error("Usage: %s <none|menu|build|carnage|results>", argv[0]);
        return;
    }

	if (strcmp(argv[1], "none") == 0)
	{
		gfx::hud.swapUI(NONE);
	}
	else if (strcmp(argv[1], "menu") == 0)
	{
		gfx::hud.swapUI(MENU);
	}
	else if (strcmp(argv[1], "build") == 0)
	{
		gfx::hud.swapUI(BUILD);
	}
	else if (strcmp(argv[1], "carnage") == 0)
	{
		gfx::hud.swapUI(CARNAGE);
	}
	else if (strcmp(argv[1], "results") == 0)
	{
		gfx::hud.swapUI(RESULTS);
	}
	else
	{
		gfx::hud.console.error("Unknown UI.");
	}
}

void Hud::swapUI(UiType which)
{
	switch (which)
	{
		case NONE:
			_currentUI = NULL;
			break;

		case MENU:

			break;

		case BUILD:

			break;

		case CARNAGE:
			_currentUI = _carnageUI;
			break;

		case RESULTS:

			break;

		default:
			break;
	}
}

void Hud::update(int ms)
{
    static bool latch = false;

    // update the current ui
    if (_currentUI != NULL) _currentUI->update(ms);

    if (io::keys['`'] && !latch)
    {
        if (_showConsole)
        {
            console.capture();
        }
        else
        {
            _showConsole = true;
        }
        latch = true;
    }
    else if (!io::keys['`'] && latch)
    {
        latch = false;
    }

    console.update(ms);
}

void Hud::draw()
{
    // switch to a 2D projection in pixel space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, global::width, 0.0, global::height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // disable lighting and depth testing
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // draw the ui
    if (_currentUI != NULL) _currentUI->draw();

    // draw the debugging console if enabled
    if (_showConsole) console.draw();

    // reenable lighting and depth testing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
