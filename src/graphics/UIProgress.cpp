#include "UIProgress.h"

UIProgress::UIProgress()
	: UIControl()
{
	_fgr = _fgg = _fgb = _fga = 1.0;
	_percent = 0;
}

UIProgress::~UIProgress()
{
	// nothing yet
}

void UIProgress::init(ProgressDir dir)
{
	UIControl::init(100, 100, 100, 100, 0.0, 0.0, 0.0, 1.0);

	_dir = dir;
}

void UIProgress::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class

	// update our children
	updateChildren(ms);
}

void UIProgress::draw()
{
	// draw the base class
	UIControl::draw();

	// draw this class
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor4f(_fgr, _fgg, _fgb, _fga);
			switch (_dir)
			{
				case LEFT:
					glVertex2f(_parent->x() + _x + percentX(), _parent->y() + _y);
					glVertex2f(_parent->x() + _x + _w, _parent->y() + _y);
					glVertex2f(_parent->x() + _x + _w, _parent->y() + _y + _h);
					glVertex2f(_parent->x() + _x + percentX(), _parent->y() + _y + _h);
					break;

				case RIGHT:
					glVertex2f(_parent->x() + _x, _parent->y() + _y);
					glVertex2f(_parent->x() + _x + percentX(), _parent->y() + _y);
					glVertex2f(_parent->x() + _x + percentX(), _parent->y() + _y + _h);
					glVertex2f(_parent->x() + _x, _parent->y() + _y + _h);
					break;

				default:
					break;
			}
		glEnd();
	glPopMatrix();

	// draw our children
	drawChildren();
}
