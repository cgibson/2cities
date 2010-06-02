#include "GameUI.h"
#include "graphics.h"

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

void GameUI::keyDown(int key, bool special)
{
	// defaults to nothing, override me
}

void GameUI::keyUp(int key, bool special)
{
	// defaults nothing, override me
}

void GameUI::mouseDown(int button)
{
	// defaults nothing, override me
}

void GameUI::mouseUp(int button)
{
	// defaults nothing, override me
}

void GameUI::mouseWheel(int direction)
{
	// defaults nothing, override me
}

void GameUI::key_down_handler(int key, bool special)
{
	GameUI *ui = gfx::hud.currentUI();
	if (ui != NULL) ui->keyDown(key, special);
}

void GameUI::key_up_handler(int key, bool special)
{
	GameUI *ui = gfx::hud.currentUI();
	if (ui != NULL) ui->keyUp(key, special);
}

void GameUI::mouse_down_handler(int button)
{
	GameUI *ui = gfx::hud.currentUI();
	if (ui != NULL) ui->mouseDown(button);
}

void GameUI::mouse_up_handler(int button)
{
	GameUI *ui = gfx::hud.currentUI();
	if (ui != NULL) ui->mouseUp(button);
}

void GameUI::mouse_wheel_handler(int direction)
{
	GameUI *ui = gfx::hud.currentUI();
	if (ui != NULL) ui->mouseWheel(direction);
}
