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
#include "../system/global.h"
#include "../system/io.h"
#include "console.h"
#include "GameUI.h"
#include "BlankUI.h"

class Hud {
public:
    Console console;

    Hud();

    void init();
    void update(int ms);
    void draw();

    // use this command to swap UI's
    void swapUI(int which);

    // shows or hides the console
    bool showConsole() const { return _showConsole; }
    void showConsole(bool show) { _showConsole = show; }

    // console commands
    static void hello(int argc, char *argv[]);
    static void exitApp(int argc, char *argv[]);
    static void clearConsole(int argc, char *argv[]);
    static void swapUIcmd(int argc, char *argv[]);
    static void networkCmds(int argc, char *argv[]);

private:
    bool _showConsole;
    GameUI *_ui;
    BlankUI _blankUI;
    // TODO: add one of each as necessary
};

#endif
