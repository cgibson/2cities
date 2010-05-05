#include "BlankUI.h"

BlankUI::BlankUI()
{
    // nothing yet
}

BlankUI::~BlankUI()
{
    // nothing yet
}

void BlankUI::init(const char *script)
{
    GameUI::init(script);
}

void BlankUI::update(int ms)
{
    // nothing yet

    // DEBUG
    _dimmed = true;
    _show = true;
}

void BlankUI::draw()
{
    GameUI::draw();
}
