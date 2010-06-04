#include "BuildUI.h"
#include "../state/BuildState.h"
#include "graphics.h"

BuildUI::BuildUI()
	: GameUI()
{
    _redResources = NULL;
    _blueResources = NULL;
    _redFullLabel = NULL;
    _blueFullLabel = NULL;
    _redFullProgress = NULL;
    _blueFullProgress = NULL;
    _typeSelect = NULL;
    _bricksMult = NULL;
	_lincolnMult = NULL;
	_bubbleMult = NULL;
	_readyButton = NULL;
	_otherPlayerReady = NULL;
}

BuildUI::~BuildUI()
{
	if (_redResources != NULL) delete _redResources;
	if (_blueResources != NULL) delete _blueResources;
	if (_redFullLabel != NULL) delete _redFullLabel;
	if (_blueFullLabel != NULL) delete _blueFullLabel;
	if (_redFullProgress != NULL) delete _redFullProgress;
	if (_blueFullProgress != NULL) delete _blueFullProgress;
	if (_typeSelect != NULL) delete _typeSelect;
	if (_bricksMult != NULL) delete _bricksMult;
	if (_lincolnMult != NULL) delete _lincolnMult;
	if (_bubbleMult != NULL) delete _bubbleMult;
	if (_readyButton != NULL) delete _readyButton;
	if (_otherPlayerReady != NULL) delete _otherPlayerReady;
}

void BuildUI::init()
{
	GameUI::init(0.0, 0.0, 0.0, 0.0);

	_typeSelect = new UIWheelChooser();
	_typeSelect->init(3, 64);
	_typeSelect->pos(global::width / 2, global::height - 115);

	_typeSelect->icon(0)->init("resources/textures/bullets_icon.bmp", NULL);
	_typeSelect->icon(0)->parent(_typeSelect);
	_typeSelect->name(0)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_typeSelect->name(0)->text("BRICK BUILDING");
	_typeSelect->name(0)->parent(_typeSelect->icon(0));

	_bricksMult = new UILabel();
	_bricksMult->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_bricksMult->text("x1");
	_bricksMult->parent(_typeSelect->icon(0));

	_typeSelect->icon(1)->init("resources/textures/shotgun_icon.bmp", NULL);
	_typeSelect->icon(1)->parent(_typeSelect);
	_typeSelect->name(1)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_typeSelect->name(1)->text("LINCOLN LOGS");
	_typeSelect->name(1)->parent(_typeSelect->icon(1));

	_lincolnMult = new UILabel();
	_lincolnMult->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_lincolnMult->text("x2");
	_lincolnMult->parent(_typeSelect->icon(1));

	_typeSelect->icon(2)->init("resources/textures/ballhemoth_icon.bmp", NULL);
	_typeSelect->icon(2)->parent(_typeSelect);
	_typeSelect->name(2)->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::CENTER);
	_typeSelect->name(2)->text("BUBBLE");
	_typeSelect->name(2)->parent(_typeSelect->icon(2));

	_bubbleMult = new UILabel();
	_bubbleMult->init("resources/fonts/sui_generis_free.ttf", 10, UILabel::CENTER);
	_bubbleMult->text("x3");
	_bubbleMult->parent(_typeSelect->icon(2));

	_typeSelect->parent(_window);

	_redResources = new UIScoreDisplay();
	_redResources->init(UIScoreDisplay::LEFT_PLAYER, UIScoreDisplay::RESOURCES);
	_redResources->pos(0, global::height - 75);
	_redResources->size(330, 75);
	_redResources->bgclr(1.0, 0.0, 0.0, 0.75);
	_redResources->parent(_window);

	_blueResources = new UIScoreDisplay();
	_blueResources->init(UIScoreDisplay::RIGHT_PLAYER, UIScoreDisplay::RESOURCES);
	_blueResources->pos(global::width - 290, global::height - 75);
	_blueResources->size(330, 75);
	_blueResources->bgclr(0.0, 0.0, 1.0, 0.75);
	_blueResources->parent(_window);

	_redFullProgress = new UIProgress();
	_redFullProgress->init(UIProgress::RIGHT);
	_redFullProgress->pos(330, global::height - 21);
	_redFullProgress->size(global::width / 2 - 330, 21);
	_redFullProgress->bgclr(1.0, 0.0, 0.0, 0.35);
	_redFullProgress->fgclr(1.0, 0.0, 0.0, 0.6);
	_redFullProgress->percent(50);
	_redFullProgress->parent(_window);

	_redFullLabel = new UILabel();
	_redFullLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::RIGHT);
	_redFullLabel->text("0%");
	_redFullLabel->parent(_redFullProgress);

	_blueFullProgress = new UIProgress();
	_blueFullProgress->init(UIProgress::LEFT);
	_blueFullProgress->pos(global::width / 2, global::height - 21);
	_blueFullProgress->size(global::width / 2 - 330, 21);
	_blueFullProgress->bgclr(0.0, 0.0, 1.0, 0.35);
	_blueFullProgress->fgclr(0.0, 0.0, 1.0, 0.6);
	_blueFullProgress->percent(50);
	_blueFullProgress->parent(_window);

	_blueFullLabel = new UILabel();
	_blueFullLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::LEFT);
	_blueFullLabel->text("0%");
	_blueFullLabel->parent(_blueFullProgress);

	_readyButton = new UIButton();
	_readyButton->init(BuildUI::readyClick);
	_readyButton->text()->init("resources/fonts/sui_generis_free.ttf", 18, UILabel::LEFT);
	_readyButton->text()->text("I'm ready!");
	_readyButton->icon()->init("resources/textures/checkmark.bmp", "resources/textures/checkmark_alpha.bmp");
	_readyButton->bgclr(0.5, 0.5, 0.5, 0.5);
	_readyButton->mouseOverClr(0.5, 0.5, 0.5, 0.75);
	_readyButton->mouseDownClr(0.5, 1.0, 0.5, 0.75);
	_readyButton->pos(global::width / 2 - 95, 20);
	_readyButton->size(190, 50);
	_readyButton->parent(_window);

	_otherPlayerReady = new UILabel();
	_otherPlayerReady->init("resources/fonts/sui_generis_free.ttf", 18, UILabel::LEFT);
	_otherPlayerReady->text("Other player is ready!");
	_otherPlayerReady->pos(20, 34);
	_otherPlayerReady->parent(_window);
}

void BuildUI::update(int ms)
{
	static int timeaccum = 0;
	timeaccum += ms;
	int amount = timeaccum % 4000;

	// keep the progress bars sized and aligned correctly
	_redFullProgress->pos(330, global::height - 21);
	_redFullProgress->size(global::width / 2 - 330, 21);
	_blueFullProgress->pos(global::width / 2, global::height - 21);
	_blueFullProgress->size(global::width / 2 - 330, 21);
	_blueFullLabel->pos(global::width / 2 - 330 - 5, 4);

	// keep the type selector "centered" in the middle of the screen (horizontally)
	_typeSelect->pos(global::width / 2, global::height - 115);

	// keep the ready button centered on the screen
	_readyButton->pos(global::width / 2 - 95, 20);

	// animate the the intensity of the other player ready indicator
	float alpha = (sinf(timeaccum / 100.0) + 1.0) / 4.0 + 0.5;
	_otherPlayerReady->fgclr(_otherPlayerReady->fgr(), _otherPlayerReady->fgg(), _otherPlayerReady->fgb(), alpha);

	// TODO: actually get resource percentage from game state here

	_redFullProgress->percent((int)(amount / 4000.0 * 100));
	snprintf(_redFullBuf, 10, "%d%%", _redFullProgress->percent());
	_redFullLabel->text(_redFullBuf);
	_redFullLabel->pos(_redFullProgress->percentX() - 5, 4);

	_blueFullProgress->percent((int)(amount / 4000.0 * 100));
	snprintf(_blueFullBuf, 10, "%d%%", _blueFullProgress->percent());
	_blueFullLabel->text(_blueFullBuf);
	_blueFullLabel->pos(_blueFullProgress->percentX() + 5, 4);

	// TODO: remove parenting the _otherPlayerReady label in the init() function
	// and instead parent it here when we detect that the game state says the
	// other player is ready

	GameUI::update(ms);

	// keep the ammo clips matched to their icons
	_bricksMult->pos(_typeSelect->icon(0)->width() / 2, -13);
	_lincolnMult->pos(_typeSelect->icon(1)->width() / 2, -13);
	_bubbleMult->pos(_typeSelect->icon(2)->width() / 2, -13);
}

void BuildUI::draw()
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

void BuildUI::ready()
{
	gfx::hud.swapUI(Hud::WAITING);
	//TODO more things?
}

void BuildUI::keyDown(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case '1':
				_typeSelect->selectItem(0);
				break;

			case '2':
				_typeSelect->selectItem(1);
				break;

			case '3':
				_typeSelect->selectItem(2);
				break;

			default:
				break;
		}
	}
}

void BuildUI::mouseWheel(int direction)
{
	if (direction == MOUSE_WHEEL_UP)
	{
		_typeSelect->prevItem();
	}
	else
	{
		_typeSelect->nextItem();
	}
}

void BuildUI::readyClick()
{
	BuildUI *ui = static_cast<BuildUI *>(gfx::hud.currentUI());
	ui->ready();
}
