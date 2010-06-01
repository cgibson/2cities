#include "UIControl.h"

UIControl::UIControl()
{
    _x = _y = 0;
    _w = _h = 100;
    _bgr = _bgg = _bgb = 0.0;
    _bga = 1.0;
}

UIControl::~UIControl()
{
    _children.clear();
}

void UIControl::init(int x, int y, int w, int h, float bgr, float bgg, float bgb, float bga)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_bgr = bgr;
	_bgg = bgg;
	_bgb = bgb;
	_bga = bga;
}

void UIControl::update(int ms)
{
	// you must explicitly call updateChildren() in your derived update() method
}

void UIControl::draw()
{
	// first draw this control
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor4f(_bgr, _bgg, _bgb, _bga);
			glVertex2f(_x, _y);
			glVertex2f(_x + _w, _y);
			glVertex2f(_x + _w, _y + _h);
			glVertex2f(_x, _y + _h);
		glEnd();
	glPopMatrix();

	// you must explicitly call drawChildren() in your derived draw() method
}

void UIControl::updateChildren(int ms)
{
	for (int i = 0; i < (int)_children.size(); i++)
	{
		_children[i]->update(ms);
	}
}

void UIControl::drawChildren()
{
	for (int i = 0; i < (int)_children.size(); i++)
	{
		_children[i]->draw();
	}
}
