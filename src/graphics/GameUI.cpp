#include "GameUI.h"

GameUI::GameUI()
{
    _window = NULL;
}

GameUI::~GameUI()
{
    if (_window != NULL) delete _window;
}

void GameUI::init(float r, float g, float b, float a)
{
	_window = new UIWindow();
	_window->init(r, g, b, a);
}

void GameUI::update(int ms)
{
	_window->update(ms);
}

void GameUI::draw()
{
	_window->draw();
}
