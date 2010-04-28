#include "console.h"

Console::Console()
{

}

void Console::init()
{

}

void Console::draw()
{
    // draw the translucent background
    glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 0.0, 0.5);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP - CONSOLE_HEIGHT);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP - CONSOLE_HEIGHT);
    glEnd();

    // draw the 1px border
    glBegin(GL_LINE_LOOP);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glVertex2i(CONSOLE_LEFT - 1, global::height - CONSOLE_TOP - 1);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH + 1, global::height - CONSOLE_TOP - 1);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH + 1, global::height - CONSOLE_TOP - 1 - CONSOLE_HEIGHT - 1);
        glVertex2i(CONSOLE_LEFT - 1, global::height - CONSOLE_TOP - 1 - CONSOLE_HEIGHT - 1);
    glEnd();

    // draw the header
    glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 0.0, 0.5);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP);
        glVertex2i(CONSOLE_LEFT + CONSOLE_WIDTH, global::height - CONSOLE_TOP - 25);
        glVertex2i(CONSOLE_LEFT, global::height - CONSOLE_TOP - 25);
    glEnd();
}

void Console::update(int ms)
{

}
