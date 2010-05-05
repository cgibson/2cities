#include "GameUI.h"

const int GameUI::NONE = 0;
const int GameUI::MENU = 1;
const int GameUI::BUILD = 2;
const int GameUI::CARNAGE = 3;
const int GameUI::BLANK = 4;

GameUI::GameUI()
{
    _dimmed = false;
    _show = false;
}

GameUI::~GameUI()
{
    // nothing yet
}

void GameUI::init(const char *script)
{
    // some lua setup stuff....
}

void GameUI::draw()
{
    if (_show && _dimmed)
    {
        // draw a half transparent quad across the whole screen
        glBegin(GL_QUADS);
            glColor4f(0.0, 0.0, 0.0, 0.5);
            glVertex2i(0, 0);
            glVertex2i(global::width, 0);
            glVertex2i(global::width, global::height);
            glVertex2i(0, global::height);
        glEnd();
    }
}
