#ifndef _HUD_H_
#define _HUH_H_

#include <GL/glut.h>
#include "../system/global.h"
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

private:
    bool _showConsole;
};

#endif
