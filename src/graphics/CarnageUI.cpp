#include "CarnageUI.h"

CarnageUI::CarnageUI()
	: GameUI()
{
    _testLabel = NULL;
    _testIcon = NULL;
    _testProgress = NULL;
}

CarnageUI::~CarnageUI()
{
	if (_testLabel != NULL) delete _testLabel;
	if (_testIcon != NULL) delete _testIcon;
	if (_testProgress != NULL) delete _testProgress;
}

void CarnageUI::init()
{
	GameUI::init(1.0, 0.0, 0.0, 0.5);

	_testLabel = new UILabel();
	_testLabel->init("resources/fonts/sui_generis_free.ttf", 28, UILabel::LEFT);
	_testLabel->shadow(true);
	_testLabel->text("AMBALAMPS!");
	_testLabel->pos(450, 300);
	_testLabel->parent(_window);

	_testIcon = new UIIcon();
	_testIcon->init("resources/textures/testimage.bmp", "resources/textures/testalpha.bmp");
	_testIcon->pos(500, 500);
	_testIcon->parent(_window);

	_testProgress = new UIProgress();
	_testProgress->init(UIProgress::LEFT);
	_testProgress->pos(100, 400);
	_testProgress->size(200, 25);
	_testProgress->bgclr(1.0, 1.0, 0.0, 0.5);
	_testProgress->fgclr(1.0, 1.0, 0.0, 0.5);
	_testProgress->percent(30);
	_testProgress->parent(_window);
}

void CarnageUI::update(int ms)
{
	static int timeaccum = 0;
	timeaccum += ms;

	GameUI::update(ms);

	int amount = timeaccum % 5000;
	_testProgress->percent((int)((amount / 5000.0) * 100));
}

void CarnageUI::draw()
{
	GameUI::draw();
}

