#include "UIButton.h"

UIButton::UIButton()
	: UIControl()
{
	_text = NULL;
	_icon = NULL;
}

UIButton::~UIButton()
{
	if (_text != NULL) delete _text;
	if (_icon != NULL) delete _icon;
}

void UIButton::init()
{
	UIControl::init(100, 100, 100, 100, 0.0, 0.0, 0.0, 1.0);

	_text = new UILabel();
	_text->parent(this);

	_icon = new UIIcon();
	_icon->parent(this);
}

void UIButton::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class

	// center the icon vertically 5 pixels from the left of the button
	_icon->pos(5, _h / 2 - _icon->height() / 2);
	_text->pos(_icon->width() + 15, _h / 2 - _text->fontSize() / 2);

	// update our children
	updateChildren(ms);
}

void UIButton::draw()
{
	// DON'T draw the base class
	UIControl::draw();

	// draw this class

	// draw our children
	drawChildren();
}
