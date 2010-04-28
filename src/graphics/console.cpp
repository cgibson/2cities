#include "console.h"

Console::Console()
{
    titleFont = NULL;
    consoleFont = NULL;
}

void Console::init()
{
    // load the fonts
    titleFont = new OGLFT::Translucent("resources/fonts/sui_generis_free.ttf", 12);
    if (titleFont->isValid())
    {
        titleFont->setForegroundColor(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        fprintf(stderr, "WARNING: Failed to load debugging console title font!\n");
    }

    consoleFont = new OGLFT::Translucent("resources/fonts/consolas.ttf", 12);
    if (!consoleFont->isValid())
    {
        fprintf(stderr, "WARNING: Failed to load debugging console monospace font!\n");
    }
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

    // draw the title
    titleFont->setHorizontalJustification(OGLFT::Face::LEFT);
    titleFont->draw(CONSOLE_LEFT + 6, global::height - CONSOLE_TOP - 25 + 6, "Debug Console");

    // draw the fps
    char buffer[10];
    snprintf(buffer, 10, "%d fps", global::fps);
    titleFont->setHorizontalJustification(OGLFT::Face::RIGHT);
    titleFont->draw(CONSOLE_LEFT + CONSOLE_WIDTH - 6, global::height - CONSOLE_TOP - 25 + 6, buffer);
}

void Console::update(int ms)
{

}
