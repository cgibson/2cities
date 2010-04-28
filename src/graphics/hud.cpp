#include "hud.h"
#include "graphics.h"

Hud::Hud()
{
    _showConsole = false; // default on
}

void Hud::init()
{
    console.init();

    // example of registering a function with a console command
    console.registerCmd("hello", Hud::hello);
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

void Hud::update(int ms)
{
    static bool latch = false;

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

    // draw the debugging console if enabled
    if (_showConsole) console.draw();

    // reenable lighting and depth testing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
