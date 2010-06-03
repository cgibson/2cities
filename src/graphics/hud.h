#ifndef _HUD_H_
#define _HUD_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../system/global.h"
#include "../system/io.h"
#include "console.h"
#include "GameUI.h"
#include "CarnageUI.h"
#include "BuildUI.h"

class Hud {
public:
	typedef enum E_UITYPE
	{
		NONE,
		MENU,
		BUILD,
		CARNAGE,
		RESULTS
	} UiType;

    Console console;

    Hud();
    ~Hud();

    void init();
    void update(int ms);
    void draw();

    // use this command to swap UI's
	void swapUI(UiType which);

    // shows or hides the console
    bool showConsole() const { return _showConsole; }
    void showConsole(bool show) { _showConsole = show; }

    // console commands
    static void exitApp(int argc, char *argv[]);
    static void clearConsole(int argc, char *argv[]);
    static void swapUIcmd(int argc, char *argv[]);

    // accessors
    GameUI *currentUI() const { return _currentUI; } // may be null!

private:
    bool _showConsole;
    GameUI *_currentUI;
    CarnageUI *_carnageUI;
    BuildUI *_buildUI;
};

#endif
