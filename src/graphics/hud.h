#ifndef _HUD_H_
#define _HUD_H_

#include <stdlib.h>
#include <GL/glut.h>
#include "../system/global.h"
#include "../system/io.h"
#include "console.h"

class Hud {
public:
    Console console;

    Hud();

    void init();
    void update(int ms);
    void draw();

    // shows or hides the console
    bool showConsole() const { return _showConsole; }
    void showConsole(bool show) { _showConsole = show; }

    // example console command
    static void hello(int argc, char *argv[]);

    // exit console command
    static void exitApp(int argc, char *argv[]);

private:
    bool _showConsole;
};

#endif
