#include "CarnageUI.h"

CarnageUI::CarnageUI()
	: GameUI()
{
    _reticle = NULL;
    _reticleTintSet = false;
    _redScore = NULL;
    _blueScore = NULL;
    _redDestructLabel = NULL;
    _blueDestructLabel = NULL;
    _redDestructProgress = NULL;
    _blueDestructProgress = NULL;
    _countdown = NULL;
    _ammoSelect = NULL;
    _bulletsRecharge = NULL;
    _shotgunRecharge = NULL;
    _ballhemothRecharge = NULL;
    _blackholeClip = NULL;
	_airstrikeClip = NULL;
	_shapeshifterClip = NULL;
	_clusterbombClip = NULL;
	_identifyLabel = NULL;
	_identifySet = false;
}

CarnageUI::~CarnageUI()
{
	if (_reticle != NULL) delete _reticle;
	if (_redScore != NULL) delete _redScore;
	if (_blueScore != NULL) delete _blueScore;
	if (_redDestructLabel != NULL) delete _redDestructLabel;
	if (_blueDestructLabel != NULL) delete _blueDestructLabel;
	if (_redDestructProgress != NULL) delete _redDestructProgress;
	if (_blueDestructProgress != NULL) delete _blueDestructProgress;
	if (_countdown != NULL) delete _countdown;
	if (_ammoSelect != NULL) delete _ammoSelect;
	if (_bulletsRecharge != NULL) delete _bulletsRecharge;
	if (_shotgunRecharge != NULL) delete _shotgunRecharge;
	if (_ballhemothRecharge != NULL) delete _ballhemothRecharge;
	if (_blackholeClip != NULL) delete _blackholeClip;
	if (_airstrikeClip != NULL) delete _airstrikeClip;
	if (_shapeshifterClip != NULL) delete _shapeshifterClip;
	if (_clusterbombClip != NULL) delete _clusterbombClip;
	if (_identifyLabel != NULL) delete _identifyLabel;
}

void CarnageUI::init()
{
	GameUI::init(0.0, 0.0, 0.0, 0.0);

	_reticle = new UIIcon();
	_reticle->init("resources/textures/reticle.bmp", "resources/textures/reticle_alpha.bmp");
	_reticle->pos(global::width / 2 - _reticle->width() / 2, global::height / 2 - _reticle->height() / 2);

	_ammoSelect = new UIWheelChooser();
	_ammoSelect->init(7, 64);
	_ammoSelect->pos(global::width / 2, global::height - 115);

	_ammoSelect->icon(0)->init("resources/textures/bullets_icon.bmp", NULL);
	_ammoSelect->icon(0)->parent(_ammoSelect);
	_ammoSelect->name(0)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(0)->text("BULLETS");
	_ammoSelect->name(0)->parent(_ammoSelect->icon(0));

	_bulletsRecharge = new UIProgress();
	_bulletsRecharge->init(UIProgress::RIGHT);
	_bulletsRecharge->bgclr(0.0, 0.0, 0.0, 0.0);
	_bulletsRecharge->fgclr(0.0, 1.0, 0.0, 1.0);
	_bulletsRecharge->percent(100);
	_bulletsRecharge->parent(_ammoSelect->icon(0));

	_ammoSelect->icon(1)->init("resources/textures/shotgun_icon.bmp", NULL);
	_ammoSelect->icon(1)->parent(_ammoSelect);
	_ammoSelect->name(1)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(1)->text("SHOTGUN");
	_ammoSelect->name(1)->parent(_ammoSelect->icon(1));

	_shotgunRecharge = new UIProgress();
	_shotgunRecharge->init(UIProgress::RIGHT);
	_shotgunRecharge->bgclr(0.0, 0.0, 0.0, 0.0);
	_shotgunRecharge->fgclr(0.0, 1.0, 0.0, 1.0);
	_shotgunRecharge->percent(100);
	_shotgunRecharge->parent(_ammoSelect->icon(1));

	_ammoSelect->icon(2)->init("resources/textures/ballhemoth_icon.bmp", NULL);
	_ammoSelect->icon(2)->parent(_ammoSelect);
	_ammoSelect->name(2)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(2)->text("BALLHEMOTH");
	_ammoSelect->name(2)->parent(_ammoSelect->icon(2));

	_ballhemothRecharge = new UIProgress();
	_ballhemothRecharge->init(UIProgress::RIGHT);
	_ballhemothRecharge->bgclr(0.0, 0.0, 0.0, 0.0);
	_ballhemothRecharge->fgclr(0.0, 1.0, 0.0, 1.0);
	_ballhemothRecharge->percent(100);
	_ballhemothRecharge->parent(_ammoSelect->icon(2));

	_ammoSelect->icon(3)->init("resources/textures/blackhole_icon.bmp", NULL);
	_ammoSelect->icon(3)->parent(_ammoSelect);
	_ammoSelect->name(3)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(3)->text("BLACK HOLE");
	_ammoSelect->name(3)->parent(_ammoSelect->icon(3));

	_blackholeClip = new UILabel();
	_blackholeClip->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_blackholeClip->text("0");
	_blackholeClip->parent(_ammoSelect->icon(3));

	_ammoSelect->icon(4)->init("resources/textures/airstrike_icon.bmp", NULL);
	_ammoSelect->icon(4)->parent(_ammoSelect);
	_ammoSelect->name(4)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(4)->text("AIR STRIKE");
	_ammoSelect->name(4)->parent(_ammoSelect->icon(4));

	_airstrikeClip = new UILabel();
	_airstrikeClip->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_airstrikeClip->text("0");
	_airstrikeClip->parent(_ammoSelect->icon(4));

	_ammoSelect->icon(5)->init("resources/textures/shapeshifter_icon.bmp", NULL);
	_ammoSelect->icon(5)->parent(_ammoSelect);
	_ammoSelect->name(5)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(5)->text("SHAPE SHIFTER");
	_ammoSelect->name(5)->parent(_ammoSelect->icon(5));

	_shapeshifterClip = new UILabel();
	_shapeshifterClip->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_shapeshifterClip->text("0");
	_shapeshifterClip->parent(_ammoSelect->icon(5));

	_ammoSelect->icon(6)->init("resources/textures/clusterbomb_icon.bmp", NULL);
	_ammoSelect->icon(6)->parent(_ammoSelect);
	_ammoSelect->name(6)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(6)->text("CLUSTER BOMB");
	_ammoSelect->name(6)->parent(_ammoSelect->icon(6));

	_clusterbombClip = new UILabel();
	_clusterbombClip->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_clusterbombClip->text("0");
	_clusterbombClip->parent(_ammoSelect->icon(6));

	_ammoSelect->parent(_window);

	_redScore = new UIScoreDisplay();
	_redScore->init(UIScoreDisplay::LEFT_PLAYER, UIScoreDisplay::SCORE);
	_redScore->pos(0, global::height - 75);
	_redScore->size(330, 75);
	_redScore->bgclr(1.0, 0.0, 0.0, 0.75);
	_redScore->parent(_window);

	_blueScore = new UIScoreDisplay();
	_blueScore->init(UIScoreDisplay::RIGHT_PLAYER, UIScoreDisplay::SCORE);
	_blueScore->pos(global::width - 290, global::height - 75);
	_blueScore->size(330, 75);
	_blueScore->bgclr(0.0, 0.0, 1.0, 0.75);
	_blueScore->parent(_window);

	_redDestructProgress = new UIProgress();
	_redDestructProgress->init(UIProgress::RIGHT);
	_redDestructProgress->pos(330, global::height - 21);
	_redDestructProgress->size(global::width / 2 - 330, 21);
	_redDestructProgress->bgclr(1.0, 0.0, 0.0, 0.35);
	_redDestructProgress->fgclr(1.0, 0.0, 0.0, 0.6);
	_redDestructProgress->percent(50);
	_redDestructProgress->parent(_window);

	_redDestructLabel = new UILabel();
	_redDestructLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::RIGHT);
	_redDestructLabel->text("0%");
	_redDestructLabel->parent(_redDestructProgress);

	_blueDestructProgress = new UIProgress();
	_blueDestructProgress->init(UIProgress::LEFT);
	_blueDestructProgress->pos(global::width / 2, global::height - 21);
	_blueDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructProgress->bgclr(0.0, 0.0, 1.0, 0.35);
	_blueDestructProgress->fgclr(0.0, 0.0, 1.0, 0.6);
	_blueDestructProgress->percent(50);
	_blueDestructProgress->parent(_window);

	_blueDestructLabel = new UILabel();
	_blueDestructLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::LEFT);
	_blueDestructLabel->text("0%");
	_blueDestructLabel->parent(_blueDestructProgress);

	_countdown = new UICountdown();
	_countdown->init();
	_countdown->pos(0, 0);
	_countdown->size(240, 75);
	_countdown->bgclr(1.0, 1.0, 1.0, 0.5);
	_countdown->parent(_window);

	_identifyLabel = new UILabel();
	_identifyLabel->init("resources/fonts/sui_generis_free.ttf", 18, UILabel::RIGHT);
	_identifyLabel->pos(global::width - 20, 20);
}

void CarnageUI::update(int ms)
{
	// keep the reticle centered
	_reticle->pos(global::width / 2 - _reticle->width() / 2, global::height  / 2 - _reticle->height() / 2);
	if (!_reticleTintSet && global::networkManager->network->getMyPlayerID() == 1)
	{
		_reticle->tint(true);
		_reticle->tintclr(1.0, 0.0, 0.0);
		_reticleTintSet = true;
		_reticle->parent(_window);
	}
	else if (!_reticleTintSet && global::networkManager->network->getMyPlayerID() == 2)
	{
		_reticle->tint(true);
		_reticle->tintclr(0.0, 0.0, 1.0);
		_reticleTintSet = true;
		_reticle->parent(_window);
	}

	// keep the progress bars sized and aligned correctly
	_redDestructProgress->pos(330, global::height - 21);
	_redDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructProgress->pos(global::width / 2, global::height - 21);
	_blueDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructLabel->pos(global::width / 2 - 330 - 5, 4);

	// keep the ammo selector "centered" in the middle of the screen (horizontally)
	_ammoSelect->pos(global::width / 2, global::height - 115);

	// keep identifier label right-aligned
	_identifyLabel->pos(global::width - 20, 20);
	if (!_identifySet && global::networkManager->network->getMyPlayerID() == 1)
	{
		_identifyLabel->fgclr(1.0, 0.0, 0.0, 1.0);
		_identifyLabel->text("You are RED.");
		_identifySet = true;
		_identifyLabel->parent(_window);
	}
	else if (!_identifySet && global::networkManager->network->getMyPlayerID() == 2)
	{
		_identifyLabel->fgclr(0.0, 0.0, 1.0, 1.0);
		_identifyLabel->text("You are BLUE.");
		_identifySet = true;
		_identifyLabel->parent(_window);
	}

	// fetch percent damage from the network api and update the UI elements accordingly
	int redPercent = global::networkManager->network->getPlayerDamage(1); // 1 = red
	int bluePercent = global::networkManager->network->getPlayerDamage(2); // 2 = blue
	if (redPercent != _redDestructProgress->percent())
	{
		_redDestructProgress->percent(redPercent);
		snprintf(_redDestructBuf, 10, "%d%%", _redDestructProgress->percent());
		_redDestructLabel->text(_redDestructBuf);
		_redDestructLabel->pos(_redDestructProgress->percentX() - 5, 4);
	}
	if (bluePercent != _blueDestructProgress->percent())
	{
		_blueDestructProgress->percent(bluePercent);
		snprintf(_blueDestructBuf, 10, "%d%%", _blueDestructProgress->percent());
		_blueDestructLabel->text(_blueDestructBuf);
		_blueDestructLabel->pos(_blueDestructProgress->percentX() + 5, 4);
	}

	// fetch player scores from the network api and update the UI elements accordingly
	int redScore = global::networkManager->network->getPlayerScore(1); // 1 = red
	int blueScore = global::networkManager->network->getPlayerScore(2); // 1 = red
	if (redScore != _redScore->score())
	{
		_redScore->score(redScore);
	}
	if (blueScore != _blueScore->score())
	{
		_blueScore->score(blueScore);
	}

	GameUI::update(ms);

	// keep the ammo progress bars matched to their icons
	_bulletsRecharge->pos(0, -10);
	_bulletsRecharge->size(_ammoSelect->icon(0)->width(), 5);
	_shotgunRecharge->pos(0, -10);
	_shotgunRecharge->size(_ammoSelect->icon(1)->width(), 5);
	_ballhemothRecharge->pos(0, -10);
	_ballhemothRecharge->size(_ammoSelect->icon(2)->width(), 5);

	// keep the ammo clips matched to their icons
	_blackholeClip->pos(_ammoSelect->icon(3)->width() / 2, -13);
	_airstrikeClip->pos(_ammoSelect->icon(4)->width() / 2, -13);
	_shapeshifterClip->pos(_ammoSelect->icon(5)->width() / 2, -13);
	_clusterbombClip->pos(_ammoSelect->icon(6)->width() / 2, -13);

	// read recharge progress from carnage state (TODO: read different recharge times for different weapons)
	CarnageState *state = static_cast<CarnageState *>(global::stateManager->currentState);
	float percent = (CarnageState::RECHARGE_TIME - state->getRechargeTimeLeft()) / (float)CarnageState::RECHARGE_TIME * 100.0;
	_bulletsRecharge->percent((int)percent);
	_bulletsRecharge->fgclr(1.0 - (percent / 100.0), (percent / 100.0),
		_bulletsRecharge->fgb(), 1.0);
	_shotgunRecharge->percent((int)percent);
	_shotgunRecharge->fgclr(1.0 - (percent / 100.0), (percent / 100.0),
		_shotgunRecharge->fgb(), 1.0);
	_ballhemothRecharge->percent((int)percent);
	_ballhemothRecharge->fgclr(1.0 - (percent / 100.0), (percent / 100.0),
		_ballhemothRecharge->fgb(), 1.0);

	// update the time remaining display
	_countdown->seconds(global::networkManager->network->getTimeToStateChange() / 1000 * -1);
}

void CarnageUI::draw()
{
	glPushMatrix();
		glBegin(GL_TRIANGLES);
			glColor4f(1.0, 0.0, 0.0, 0.75);
			glVertex2i(330 - 14, global::height - 21);
			glVertex2i(330, global::height - 21);
			glVertex2i(330, global::height);
			glColor4f(0.0, 0.0, 1.0, 0.75);
			glVertex2i(global::width - 330, global::height - 21);
			glVertex2i(global::width - 330 + 14, global::height - 21);
			glVertex2i(global::width - 330, global::height);
		glEnd();
	glPopMatrix();

	GameUI::draw();
}

void CarnageUI::keyDown(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case '1':
				_ammoSelect->selectItem(0);
				break;

			case '2':
				_ammoSelect->selectItem(1);
				break;

			case '3':
				_ammoSelect->selectItem(2);
				break;

			case '4':
				_ammoSelect->selectItem(3);
				break;

			case '5':
				_ammoSelect->selectItem(4);
				break;

			case '6':
				_ammoSelect->selectItem(5);
				break;

			case '7':
				_ammoSelect->selectItem(6);
				break;

			default:
				break;
		}
	}
}

void CarnageUI::mouseWheel(int direction)
{
	if (direction == MOUSE_WHEEL_UP)
	{
		_ammoSelect->prevItem();
	}
	else
	{
		_ammoSelect->nextItem();
	}
}
