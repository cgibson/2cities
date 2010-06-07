#include "ResultsUI.h"
#include "graphics.h"

ResultsUI::ResultsUI()
	: GameUI()
{
	_winLabel = NULL;
	_redScore = NULL;
	_blueScore = NULL;
	_rematchButton = NULL;
	_menuButton = NULL;
}

ResultsUI::~ResultsUI()
{
	if (_winLabel != NULL) delete _winLabel;
	if (_redScore != NULL) delete _redScore;
	if (_blueScore != NULL) delete _blueScore;
	if (_rematchButton != NULL) delete _rematchButton;
	if (_menuButton != NULL) delete _menuButton;
}

void ResultsUI::init()
{
	GameUI::init(0.0, 0.0, 0.0, 0.5);

	_winLabel = new UILabel();
	_winLabel->init("resources/fonts/sui_generis_free.ttf", 64, UILabel::CENTER);
	_winLabel->pos(global::width / 2, global::height / 2 + 50);
	_winLabel->fgclr(1.0, 1.0, 1.0, 1.0);
	_winLabel->text("<who> Wins!");
	_winLabel->parent(_window);

	_redScore = new UILabel();
	_redScore->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::RIGHT);
	_redScore->pos(global::width / 2 - 50, global::height / 2 - 25);
	_redScore->fgclr(1.0, 0.0, 0.0, 1.0);
	_redScore->text("000000");
	_redScore->parent(_window);

	_blueScore = new UILabel();
	_blueScore->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::LEFT);
	_blueScore->pos(global::width / 2 + 50, global::height / 2 - 25);
	_blueScore->fgclr(0.0, 0.0, 1.0, 1.0);
	_blueScore->text("000000");
	_blueScore->parent(_window);
}

void ResultsUI::update(int ms)
{
	GameUI::update(ms);
}

void ResultsUI::draw()
{
	GameUI::draw();
}

