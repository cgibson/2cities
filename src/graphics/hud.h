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

class Hud {
public:
    Console console;

    Hud();

    void init();
    void update(int ms);
    void draw();

    // use this command to swap UI's
	void swapUI(const char *script);

    // shows or hides the console
    bool showConsole() const { return _showConsole; }
    void showConsole(bool show) { _showConsole = show; }

    // access uis
    GameUI *getUI(int i) const { return _allUIs[i]; }
	int numUIs() const { return (int)_allUIs.size(); }

    // console commands
    static void exitApp(int argc, char *argv[]);
    static void clearConsole(int argc, char *argv[]);
    static void swapUIcmd(int argc, char *argv[]);

private:
    bool _showConsole;
	std::vector<GameUI *> _allUIs;
	GameUI *_currentUI;
	char *_unloadUI;
	bool _unloadingUI;
	GameUI *loadUI(const char *script);
	void unloadUI();
};

#endif
