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
	_redScore->text("");
	_redScore->parent(_window);

	_blueScore = new UILabel();
	_blueScore->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::LEFT);
	_blueScore->pos(global::width / 2 + 50, global::height / 2 - 25);
	_blueScore->fgclr(0.0, 0.0, 1.0, 1.0);
	_blueScore->text("");
	_blueScore->parent(_window);

	/*_rematchButton = new UIButton();
	_rematchButton->init(ResultsUI::rematchClick);
	_rematchButton->text()->init("resources/fonts/sui_generis_free.ttf", 18, UILabel::LEFT);
	_rematchButton->text()->text("Rematch!");
	_rematchButton->icon()->init("resources/textures/checkmark.bmp", "resources/textures/checkmark_alpha.bmp");
	_rematchButton->bgclr(0.5, 0.5, 0.5, 0.5);
	_rematchButton->mouseOverClr(0.5, 0.5, 0.5, 0.75);
	_rematchButton->mouseDownClr(0.5, 1.0, 0.5, 0.75);
	_rematchButton->pos(global::width / 2 - 95, 20);
	_rematchButton->size(190, 50);
	_rematchButton->parent(_window);*/
}

void ResultsUI::update(int ms)
{
	// keep everything aligned
	_winLabel->pos(global::width / 2, global::height / 2 + 50);
	_redScore->pos(global::width / 2 - 50, global::height / 2 - 25);
	_blueScore->pos(global::width / 2 + 50, global::height / 2 - 25);

	// fetch player scores from the network api and update the UI elements accordingly
	int redScore = global::networkManager->network->getPlayerScore(1); // 1 = red
	int blueScore = global::networkManager->network->getPlayerScore(2); // 2 = blue
	snprintf(_redBuf, 10, "%d", redScore);
	_redScore->text(_redBuf);
	snprintf(_blueBuf, 10, "%d", blueScore);
	_blueScore->text(_blueBuf);

	// keep the win text up to date
	if (redScore > blueScore)
	{
		_winLabel->text("Red wins!");
	}
	else if (blueScore > redScore)
	{
		_winLabel->text("Blue wins!");
	}
	else
	{
		_winLabel->text("Tie!");
	}

	GameUI::update(ms);
}

void ResultsUI::draw()
{
	GameUI::draw();
}
