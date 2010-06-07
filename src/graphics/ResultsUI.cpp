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
	_gauntletLabel = NULL;
	_countdown = NULL;
	_showGauntlet = false;
}

ResultsUI::~ResultsUI()
{
	if (_winLabel != NULL) delete _winLabel;
	if (_redScore != NULL) delete _redScore;
	if (_blueScore != NULL) delete _blueScore;
	if (_rematchButton != NULL) delete _rematchButton;
	if (_menuButton != NULL) delete _menuButton;
	if (_gauntletLabel != NULL) delete _gauntletLabel;
	if (_countdown != NULL) delete _countdown;
}

void ResultsUI::init()
{
	GameUI::init(0.0, 0.0, 0.0, 0.5);

	_winLabel = new UILabel();
	_winLabel->init("resources/fonts/sui_generis_free.ttf", 64, UILabel::CENTER);
	_winLabel->pos(global::width / 2, global::height / 2 + 150);
	_winLabel->fgclr(1.0, 1.0, 1.0, 1.0);
	_winLabel->text("<who> Wins!");
	_winLabel->parent(_window);

	_redScore = new UILabel();
	_redScore->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::RIGHT);
	_redScore->pos(global::width / 2 - 50, global::height / 2 + 50);
	_redScore->fgclr(1.0, 0.0, 0.0, 1.0);
	_redScore->text("");
	_redScore->parent(_window);

	_blueScore = new UILabel();
	_blueScore->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::LEFT);
	_blueScore->pos(global::width / 2 + 50, global::height / 2 + 50);
	_blueScore->fgclr(0.0, 0.0, 1.0, 1.0);
	_blueScore->text("");
	_blueScore->parent(_window);

	_rematchButton = new UIButton();
	_rematchButton->init(ResultsUI::rematchClick);
	_rematchButton->text()->init("resources/fonts/sui_generis_free.ttf", 18, UILabel::LEFT);
	_rematchButton->text()->text("Rematch!");
	_rematchButton->icon()->init("resources/textures/checkmark.bmp", "resources/textures/checkmark_alpha.bmp");
	_rematchButton->bgclr(0.5, 0.5, 0.5, 0.5);
	_rematchButton->mouseOverClr(0.5, 0.5, 0.5, 0.75);
	_rematchButton->mouseDownClr(0.5, 1.0, 0.5, 0.75);
	_rematchButton->pos(global::width / 2 - 240, global::height / 2 - 200);
	_rematchButton->size(220, 50);
	_rematchButton->parent(_window);

	_menuButton = new UIButton();
	_menuButton->init(ResultsUI::menuClick);
	_menuButton->text()->init("resources/fonts/sui_generis_free.ttf", 18, UILabel::LEFT);
	_menuButton->text()->text("Main Menu");
	_menuButton->icon()->init("resources/textures/checkmark.bmp", "resources/textures/checkmark_alpha.bmp");
	_menuButton->bgclr(0.5, 0.5, 0.5, 0.5);
	_menuButton->mouseOverClr(0.5, 0.5, 0.5, 0.75);
	_menuButton->mouseDownClr(0.5, 1.0, 0.5, 0.75);
	_menuButton->pos(global::width / 2 + 20, global::height / 2 - 200);
	_menuButton->size(220, 50);
	_menuButton->parent(_window);

	_gauntletLabel = new UILabel();
	_gauntletLabel->init("resources/fonts/sui_generis_free.ttf", 24, UILabel::CENTER);
	_gauntletLabel->pos(global::width / 2, global::height / 2 - 50);
	_gauntletLabel->fgclr(1.0, 1.0, 1.0, 1.0);
	_gauntletLabel->text("Your opponent has thrown down the gauntlet!");
	_gauntletLabel->parent(_window);

	_countdown = new UICountdown();
	_countdown->init();
	_countdown->pos(0, 0);
	_countdown->size(240, 75);
	_countdown->bgclr(1.0, 1.0, 1.0, 0.5);
	_countdown->parent(_window);
}

void ResultsUI::update(int ms)
{
	static int timeaccum = 0;
	timeaccum += ms;

	// keep everything aligned
	_winLabel->pos(global::width / 2, global::height / 2 + 150);
	_redScore->pos(global::width / 2 - 50, global::height / 2 + 50);
	_blueScore->pos(global::width / 2 + 50, global::height / 2 + 50);
	_rematchButton->pos(global::width / 2 - 240, global::height / 2 - 200);
	_menuButton->pos(global::width / 2 + 20, global::height / 2 - 200);
	_gauntletLabel->pos(global::width / 2, global::height / 2 - 50);

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

	// check if the other player is ready and display flasher if they are
	std::vector<Client *> clients = global::networkManager->network->getPlayers();
	if (clients.size() >= 2)
	{
		int myPlayerID = global::networkManager->network->getMyPlayerID();
		if ((myPlayerID == 1 && global::networkManager->network->getPlayerReady(2) && !_showGauntlet) ||
			 (myPlayerID == 2 && global::networkManager->network->getPlayerReady(1) && !_showGauntlet))
		{
			_showGauntlet = true;
		}
	}

	// animate the the intensity of the other player ready indicator
	if (_showGauntlet)
	{
		float alpha = (sinf(timeaccum / 100.0) + 1.0) / 4.0 + 0.5;
		_gauntletLabel->fgclr(_gauntletLabel->fgr(), _gauntletLabel->fgg(), _gauntletLabel->fgb(), alpha);
	}
	else
	{
		_gauntletLabel->fgclr(_gauntletLabel->fgr(), _gauntletLabel->fgg(), _gauntletLabel->fgb(), 0.0);
	}

	// update the time remaining display
	_countdown->seconds(global::networkManager->network->getTimeToStateChange() / 1000 * -1);

	GameUI::update(ms);
}

void ResultsUI::draw()
{
	GameUI::draw();
}

void ResultsUI::rematch()
{
	gfx::hud.swapUI(Hud::WAITING);
	global::networkManager->network->setPlayerReady(1);
}

void ResultsUI::menu()
{
	global::networkManager->network->serverDisconnect();
}

void ResultsUI::reset()
{
	_showGauntlet = false;
}

void ResultsUI::rematchClick()
{
	ResultsUI *ui = static_cast<ResultsUI *>(gfx::hud.currentUI());
	ui->rematch();
}

void ResultsUI::menuClick()
{
	ResultsUI *ui = static_cast<ResultsUI *>(gfx::hud.currentUI());
	ui->menu();
}
