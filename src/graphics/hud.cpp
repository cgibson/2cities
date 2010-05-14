#include "hud.h"
#include "graphics.h"

// TEMP INCLUDE HERE
#include "../network/NetworkSystem.h"
#include "../network/NetworkPrivate.h"
#include "../network/NetworkClient.h"

Hud::Hud()
{
    _showConsole = false; // default off
    _ui = NULL; // none
}

void Hud::init()
{
    console.init();

    // initialize each ui we're going to use
    _blankUI.init("resources/blank.lua");
    // TODO: add more as necessary

    // register commands with the console
    console.registerCmd("hello", Hud::hello);
    console.registerCmd("exit", Hud::exitApp);
    console.registerCmd("clear", Hud::clearConsole);
    console.registerCmd("swapui", Hud::swapUIcmd);
    console.registerCmd("connect", Hud::networkCmds);
    console.registerCmd("network", Hud::networkCmds);
}

void Hud::hello(int argc, char *argv[])
{
    if (argc != 2)
    {
        gfx::hud.console.error("Usage: %s <your name>", argv[0]);
        return;
    }

    gfx::hud.console.info("Hello, %s!", argv[1]);
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
        gfx::hud.console.error("Usage: %s <ui name>", argv[0]);
        return;
    }

    if (strcmp(argv[1], "none") == 0)
    {
        gfx::hud.swapUI(GameUI::NONE);
    }
    else if (strcmp(argv[1], "blank") == 0)
    {
        gfx::hud.swapUI(GameUI::BLANK);
    }
    // TODO: add more as necessary
    else
    {
        gfx::hud.console.error("Unknown UI name.");
    }
}

void Hud::swapUI(int which)
{
    if (which == GameUI::NONE)
    {
        _ui = NULL;
    }
    else if (which == GameUI::BLANK)
    {
        _ui = &_blankUI;
    }
    // TODO: add more as necessary
}

void Hud::networkCmds(int argc, char *argv[]) {
	if(!strcmp(argv[0],"connect")) {
		int results = 0;
		if (!strcmp(argv[1],"local")) {
			results = network->connectServer("127.0.0.1", 5060);
		}
		else if (argc != 3) {
			gfx::hud.console.error("Usage: %s <host ip> <host port>", argv[0]);
			return;
		}
		else {
			results = network->connectServer(argv[1], atoi(argv[2]));
		}

		if(results)
			gfx::hud.console.info("Connection Successful!");
		else
			gfx::hud.console.error("Connection Failure!");

		return;
	}

	if(!strcmp(argv[0],"network")) {
		if (argc != 2) {
			gfx::hud.console.error("Usage: %s <private | client>", argv[0]);
			return;
		}

		if(!strcmp(argv[1],"private")) {
			NetworkSystem *oldNtwk = network;
			network = new NetworkPrivate();
			//delete oldNtwk;
			network->loadLevel("resources/test.lvl");
		}
		else if(!strcmp(argv[1],"client")) {
			NetworkSystem *oldNtwk = network;
			network = new NetworkClient();
			//delete oldNtwk;
		}
	}
}

void Hud::update(int ms)
{
    static bool latch = false;

    // update the ui
    if (_ui != NULL) _ui->update(ms);

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
    if (_ui != NULL) _ui->draw();

    // draw the debugging console if enabled
    if (_showConsole) console.draw();

    // reenable lighting and depth testing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
