#include "hud.h"
#include "graphics.h"

Hud::Hud()
{
    _showConsole = false; // default off
    _currentUI = NULL; // none
    _unloadUI = NULL;
    _unloadingUI = false;
}

void Hud::init()
{
    console.init();

	// preload our ui's (on the fly loading should only be
	// used for debugging purposes)
	//_allUIs.push_back(new Game());
	//_allUIs[_allUIs.size() - 1].init("testui.lua");
    // TODO: add more as necessary

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
        gfx::hud.console.error("Usage: %s <ui.lua or NULL>", argv[0]);
        return;
    }

    if (strcmp(argv[1], "NULL") == 0)
    {
		gfx::hud.swapUI(NULL);
		gfx::hud.console.info("Cleared screen UI.");
    }
    else
	{
		gfx::hud.swapUI(argv[1]);
	}
}

void Hud::swapUI(const char *script)
{
	if (script == NULL)
	{
		_unloadUI = NULL;
	}
	else
	{
		_unloadUI = (char *)malloc(sizeof(char) * (strlen(script) + 1));
		memcpy(_unloadUI, script, strlen(script));
		_unloadUI[strlen(script)] = '\0';
	}

	if (_currentUI != NULL) _currentUI->unload();
	_unloadingUI = true;
}

void Hud::unloadUI()
{
	if (_currentUI != NULL) _currentUI->reset();

	if (_unloadUI == NULL)
	{
		_currentUI = NULL;
		_unloadingUI = false;
		return;
	}

	// check to see if it's already loaded
	int location = -1;
	for (int i = 0; i < (int)_allUIs.size(); i++)
	{
		if (strcmp(_allUIs[i]->script(), _unloadUI) == 0)
		{
			location = i;
			break;
		}
	}

	if (location >= 0)
	{
		// if we're swapping to itself, that means we should reload it instead
		// of doing nothing

		if ((_currentUI != NULL) &&
			(strcmp(_currentUI->script(), _allUIs[location]->script()) == 0))
		{
			console.info("Reloading %s", _unloadUI);
			_currentUI = loadUI(_unloadUI);
			console.info("Done.");
		}
		else
		{
			console.info("Swapping UI to %s", _unloadUI);
			_currentUI = _allUIs[location];
			console.info("Done.");
		}
	}
	else
	{
		// does it exist on disk?
		int dir_len = strlen(UI_SCRIPT_DIR);
		int script_len = strlen(_unloadUI);
		char *filename = (char *)malloc(sizeof(char) * (dir_len + script_len + 1));
		memcpy(filename, UI_SCRIPT_DIR, dir_len);
		memcpy(filename + dir_len, _unloadUI, script_len);
		filename[dir_len + script_len] = '\0';
		if (FILE *fp = fopen(filename, "r"))
		{
			// yep, load the script and set the current ui
			fclose(fp);
			console.info("Loading %s", _unloadUI);
			_currentUI = loadUI(_unloadUI);
			console.info("Done.");
		}
		else
		{
			console.error("UI script not found: %s", filename);
		}
		free(filename);
	}

	if (_unloadUI != NULL)
	{
		free(_unloadUI);
		_unloadUI = NULL;
	}
	_unloadingUI = false;
}

GameUI *Hud::loadUI(const char *script)
{
	// sanity check
	if (script == NULL) return NULL;

	// create the new ui
	GameUI *ui = new GameUI(script);

	// are we replacing (reloading) an existing ui?
	int location = -1;
	for (int i = 0; i < (int)_allUIs.size(); i++)
	{
		if (strcmp(_allUIs[i]->script(), script) == 0)
		{
			location = i;
			break;
		}
	}

	if (location >= 0)
	{
		// replace the old ui in the list
		GameUI *oldUI = _allUIs[location];
		_allUIs[location] = ui;

		// replace the current ui if it's current
		if (_currentUI == oldUI) _currentUI = ui;

		// destruct the old one
		delete oldUI;

		console.info("Replaced UI in slot %d.", location);
	}
	else
	{
		// add the ui to the end of the list
		_allUIs.push_back(ui);
		console.info("Created new UI in slot %d.", _allUIs.size() - 1);
	}

	// initialize the ui
	ui->init();

	return ui;
}

void Hud::update(int ms)
{
    static bool latch = false;

    // update the current ui only
    if (_currentUI != NULL) _currentUI->update(ms);

    // are we unloading the ui?
    if (_unloadingUI)
    {
		if (_currentUI == NULL)
		{
			// NULL ui can't respond with done, so go ahead and unload the UI
			unloadUI();
		}
		else
		{
			if (_currentUI->done())
			{
				// the current UI is ready to be unloaded
				unloadUI();
			}
		}
	}

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
