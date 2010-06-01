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
}

void CarnageUI::update(int ms)
{
	static int timeaccum = 0;
	timeaccum += ms;
	int amount = timeaccum % 4000;

	_reticle->pos(global::width / 2 - _reticle->width() / 2, global::height  / 2 - _reticle->height() / 2);
	_reticle->rotAngle((timeaccum / 1000.0) * 15.0);
	if (_reticle->rotAngle() > 360.0) _reticle->rotAngle(_reticle->rotAngle() - 360.0);

	_redDestructProgress->pos(330, global::height - 21);
	_redDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructProgress->pos(global::width / 2, global::height - 21);
	_blueDestructProgress->size(global::width / 2 - 330, 21);
	_blueDestructLabel->pos(global::width / 2 - 330 - 5, 4);

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

