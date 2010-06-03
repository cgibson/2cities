#include "UIButton.h"

UIButton::UIButton()
	: UIControl()
{
	_text = NULL;
	_icon = NULL;
	_state = OUT;
	_mor = _mog = _mob = _moa = 1.0;
	_mdr = _mdg = _mdb = _mda = 1.0;
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

	// center the text vertically and place it 10 pixels left of the icon
	_text->pos(_icon->width() + 15, _h / 2 - _text->fontSize() / 2);

	// update state of the button
	if (!io::captured)
	{
		if (io::mouse_x > x() && io::mouse_x < x() + _w &&
			(global::height - io::mouse_y) > y() && (global::height - io::mouse_y) < y() + _h)
		{
			if (io::mouse_buttons[MOUSE_LEFT])
			{
				_state = OVER;
			}
			else
			{
				_state = DOWN;
			}
		}
		else
		{
			_state = OUT;
		}
	}
	else
	{
		_state = OUT;
	}

	// update our children
	updateChildren(ms);
}

void UIButton::draw()
{
	if (_state == OVER)
	{
		float r, g, b, a;
		r = _bgr; g = _bgg; b = _bgb; a = _bga;
		_bgr = _mor; _bgg = _mog; _bgb = _mob; _bga = _moa;
		UIControl::draw();
		_bgr = r; _bgg = g; _bgb = b; _bga = a;
	}
	else if (_state == DOWN)
	{
		float r, g, b, a;
		r = _bgr; g = _bgg; b = _bgb; a = _bga;
		_bgr = _mdr; _bgg = _mdg; _bgb = _mdb; _bga = _mda;
		UIControl::draw();
		_bgr = r; _bgg = g; _bgb = b; _bga = a;
	}
	else
	{
		UIControl::draw();
	}

	// draw our children
	drawChildren();
}
