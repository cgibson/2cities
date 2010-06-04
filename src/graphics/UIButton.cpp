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

void UIButton::init(void (*click_callback)())
{
	UIControl::init(100, 100, 100, 100, 0.0, 0.0, 0.0, 1.0);

	_click_callback = click_callback;

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
				if (_state == DOWN)
				{
					// click! dispatch click callback (if not null)
					if (_click_callback != NULL) _click_callback();
				}
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
	// DON'T draw base class
	//UIControl::draw();

	float r, g, b, a;
	switch (_state)
	{
		case OVER:
			r = _mor; g = _mog; b = _mob; a = _moa;
			break;

		case DOWN:
			r = _mdr; g = _mdg; b = _mdb; a = _mda;
			break;

		default:
			r = _bgr; g = _bgg; b = _bgb; a = _bga;
			break;
	}

	glPushMatrix();
		glColor4f(r, g, b, a);

		glBegin(GL_QUADS);
			// middle quad
			glVertex2i(_x, _y + 10);
			glVertex2i(_x + _w, _y + 10);
			glVertex2i(_x + _w, _y + _h - 10);
			glVertex2i(_x, _y + _h - 10);

			// top quad
			glVertex2i(_x + 10, _y + _h - 10);
			glVertex2i(_x + _w - 10, _y + _h - 10);
			glVertex2i(_x + _w - 10, _y + _h);
			glVertex2i(_x + 10, _y + _h);

			// bottom quad
			glVertex2i(_x + 10, _y);
			glVertex2i(_x + _w - 10, _y);
			glVertex2i(_x + _w - 10, _y + 10);
			glVertex2i(_x + 10, _y + 10);
		glEnd();

		glBegin(GL_TRIANGLES);
			// top left
			glVertex2i(_x, _y + _h - 10);
			glVertex2i(_x + 10, _y + _h - 10);
			glVertex2i(_x + 10, _y + _h);

			// top right
			glVertex2i(_x + _w - 10, _y + _h - 10);
			glVertex2i(_x + _w, _y + _h - 10);
			glVertex2i(_x + _w - 10, _y + _h);

			// bottom left
			glVertex2i(_x, _y + 10);
			glVertex2i(_x + 10, _y + 10);
			glVertex2i(_x + 10, _y);

			// bottom right
			glVertex2i(_x + _w - 10, _y + 10);
			glVertex2i(_x + _w, _y + 10);
			glVertex2i(_x + _w - 10, _y);
		glEnd();
	glPopMatrix();

	// draw our children
	drawChildren();
}
