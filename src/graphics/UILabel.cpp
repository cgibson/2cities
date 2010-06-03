#include "UILabel.h"

const int UILabel::DROP_X = 3;
const int UILabel::DROP_Y = 3;

UILabel::UILabel()
	: UIControl()
{
	_fgr = 1.0;
	_fgg = 1.0;
	_fgb = 1.0;
	_fga = 1.0;
	_shadow = false;
	_font = NULL;
	_text = NULL;
	_fontSize = 12;
}

UILabel::~UILabel()
{
	if (_font != NULL) delete _font;
	if (_text != NULL) free(_text);
}

void UILabel::init(const char *font, int size, TextJustify justify)
{
	UIControl::init(2, 2, 0, 0, 0.0, 0.0, 0.0, 0.0);

	_fontSize = size;

	_font = new OGLFT::Translucent(font, _fontSize);
	if (!_font->isValid())
	{
		fprintf(stderr, "Failed to load font %s!\n", font);
		exit(EXIT_FAILURE);
	}
	_font->setForegroundColor(_fgr, _fgg, _fgb, _fga);
	switch (justify)
	{
		case LEFT:
			_font->setHorizontalJustification(OGLFT::Face::LEFT);
			break;

		case CENTER:
			_font->setHorizontalJustification(OGLFT::Face::CENTER);
			break;

		case RIGHT:
			_font->setHorizontalJustification(OGLFT::Face::RIGHT);
			break;

		default:
			break;
	}
}

void UILabel::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class

	// update our children
	updateChildren(ms);
}

void UILabel::draw()
{
	// draw the base class
	UIControl::draw();

	// draw this class
	if (_shadow)
	{
		_font->setForegroundColor(0.0, 0.0, 0.0, 0.5);
		_font->draw(_parent->x() + _x + DROP_X, _parent->y() + _y - DROP_Y, _text);
	}

	_font->setForegroundColor(_fgr, _fgg, _fgb, _fga);
	_font->draw(_parent->x() + _x, _parent->y() + _y, _text);

	// draw our children
	drawChildren();
}
