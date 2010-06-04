#include "WaitingUI.h"

WaitingUI::WaitingUI()
	: GameUI()
{
	_waitingLabel = NULL;
	_waitingIcon = NULL;
}

WaitingUI::~WaitingUI()
{
	if (_waitingLabel != NULL) delete _waitingLabel;
	if (_waitingIcon != NULL) delete _waitingIcon;
}

void WaitingUI::init()
{
	GameUI::init(1.0, 1.0, 1.0, 0.7);

	_waitingLabel = new UILabel();
	_waitingLabel->init("resources/fonts/sui_generis_free.ttf", 44, UILabel::CENTER);
	_waitingLabel->text("Waiting for other player...");
	_waitingLabel->pos(global::width / 2, global::height / 2);
	_waitingLabel->fgclr(0.0, 0.0, 0.0, 1.0);
	_waitingLabel->parent(_window);

	_waitingIcon = new UIIcon();
	_waitingIcon->init("resources/textures/loader.bmp", "resources/textures/loader_alpha.bmp");
	_waitingIcon->pos(global::width / 2 - _waitingIcon->width() / 2, global::height / 2 - 50);
	_waitingIcon->parent(_window);
}

void WaitingUI::update(int ms)
{
	// keep the label and icon aligned correctly
	_waitingLabel->pos(global::width / 2, global::height / 2);
	_waitingIcon->pos(global::width / 2 - _waitingIcon->width() / 2, global::height / 2 - 50);

	// animate the waiting loader icon
	_waitingIcon->rotAngle(_waitingIcon->rotAngle() - ms * 360.0 / 1000.0);
	if (_waitingIcon->rotAngle() < 0.0) _waitingIcon->rotAngle(_waitingIcon->rotAngle() + 360.0);

	GameUI::update(ms);
}

void WaitingUI::draw()
{
	GameUI::draw();
}
