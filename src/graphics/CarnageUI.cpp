#include "CarnageUI.h"

CarnageUI::CarnageUI()
	: GameUI()
{
    _reticle = NULL;
    _redScore = NULL;
    _blueScore = NULL;
    _redDestructLabel = NULL;
    _blueDestructLabel = NULL;
    _redDestructProgress = NULL;
    _blueDestructProgress = NULL;
    _countdown = NULL;
    _ammoSelect = NULL;
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
}

void CarnageUI::init()
{
	GameUI::init(0.0, 0.0, 0.0, 0.0);

	_reticle = new UIIcon();
	_reticle->init("resources/textures/reticle.bmp", "resources/textures/reticle.bmp");
	_reticle->pos(global::width / 2 - _reticle->width() / 2, global::height / 2 - _reticle->height() / 2);
	_reticle->tint(true);
	_reticle->tintclr(1.0, 0.0, 0.0); // TODO: change this to the actual player's color!
	_reticle->parent(_window);

	_redScore = new UIScoreDisplay();
	_redScore->init(UIScoreDisplay::LEFT_PLAYER);
	_redScore->pos(0, global::height - 75);
	_redScore->size(330, 75);
	_redScore->bgclr(1.0, 0.0, 0.0, 0.75);
	_redScore->parent(_window);

	_blueScore = new UIScoreDisplay();
	_blueScore->init(UIScoreDisplay::RIGHT_PLAYER);
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

	_ammoSelect = new UIWheelChooser();
	_ammoSelect->init(7, 64);
	_ammoSelect->pos(global::width / 2, global::height - 115);

	_ammoSelect->icon(0)->init("resources/textures/bullets_icon.bmp", NULL);
	_ammoSelect->icon(0)->parent(_ammoSelect);
	_ammoSelect->name(0)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(0)->text("BULLETS");
	_ammoSelect->name(0)->parent(_ammoSelect->icon(0));

	_ammoSelect->icon(1)->init("resources/textures/shotgun_icon.bmp", NULL);
	_ammoSelect->icon(1)->parent(_ammoSelect);
	_ammoSelect->name(1)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(1)->text("SHOTGUN");
	_ammoSelect->name(1)->parent(_ammoSelect->icon(1));

	_ammoSelect->icon(2)->init("resources/textures/ballhemoth_icon.bmp", NULL);
	_ammoSelect->icon(2)->parent(_ammoSelect);
	_ammoSelect->name(2)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(2)->text("BALLHEMOTH");
	_ammoSelect->name(2)->parent(_ammoSelect->icon(2));

	_ammoSelect->icon(3)->init("resources/textures/blackhole_icon.bmp", NULL);
	_ammoSelect->icon(3)->parent(_ammoSelect);
	_ammoSelect->name(3)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(3)->text("BLACK HOLE");
	_ammoSelect->name(3)->parent(_ammoSelect->icon(3));

	_ammoSelect->icon(4)->init("resources/textures/airstrike_icon.bmp", NULL);
	_ammoSelect->icon(4)->parent(_ammoSelect);
	_ammoSelect->name(4)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(4)->text("AIR STRIKE");
	_ammoSelect->name(4)->parent(_ammoSelect->icon(4));

	_ammoSelect->icon(5)->init("resources/textures/shapeshifter_icon.bmp", NULL);
	_ammoSelect->icon(5)->parent(_ammoSelect);
	_ammoSelect->name(5)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(5)->text("SHAPE SHIFTER");
	_ammoSelect->name(5)->parent(_ammoSelect->icon(5));

	_ammoSelect->icon(6)->init("resources/textures/clusterbomb_icon.bmp", NULL);
	_ammoSelect->icon(6)->parent(_ammoSelect);
	_ammoSelect->name(6)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_ammoSelect->name(6)->text("CLUSTER BOMB");
	_ammoSelect->name(6)->parent(_ammoSelect->icon(6));

	_ammoSelect->parent(_window);
}

void CarnageUI::update(int ms)
{
	static int timeaccum = 0;
	timeaccum += ms;
	int amount = timeaccum % 4000;

	// keep the reticle centered and slowly rotate it
	_reticle->pos(global::width / 2 - _reticle->width() / 2, global::height  / 2 - _reticle->height() / 2);
	_reticle->rotAngle((timeaccum / 1000.0) * 15.0);
	if (_reticle->rotAngle() > 360.0) _reticle->rotAngle(_reticle->rotAngle() - 360.0);

	// keep the progress bars sized and aligned correctly
	_redDestructProgress->pos(330, global::height - 21);
	_redDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructProgress->pos(global::width / 2, global::height - 21);
	_blueDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructLabel->pos(global::width / 2 - 330 - 5, 4);

	// keep the ammo selector "centered" in the middle of the screen (horizontally)
	_ammoSelect->pos(global::width / 2, global::height - 115);

	// TODO: actually get destruction percentage from game state here

	_redDestructProgress->percent((int)(amount / 4000.0 * 100));
	snprintf(_redDestructBuf, 10, "%d%%", _redDestructProgress->percent());
	_redDestructLabel->text(_redDestructBuf);
	_redDestructLabel->pos(_redDestructProgress->percentX() - 5, 4);

	_blueDestructProgress->percent((int)(amount / 4000.0 * 100));
	snprintf(_blueDestructBuf, 10, "%d%%", _blueDestructProgress->percent());
	_blueDestructLabel->text(_blueDestructBuf);
	_blueDestructLabel->pos(_blueDestructProgress->percentX() + 5, 4);

	GameUI::update(ms);
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
