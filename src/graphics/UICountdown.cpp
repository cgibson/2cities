#include "UICountdown.h"

UICountdown::UICountdown()
	: UIControl()
{
	_explainLabel = NULL;
	_timeLabel = NULL;
	_seconds = 0;
}

UICountdown::~UICountdown()
{
	if (_explainLabel != NULL) delete _explainLabel;
	if (_timeLabel != NULL) delete _timeLabel;
}

void UICountdown::init()
{
	UIControl::init(100, 100, 100, 100, 0.0, 0.0, 0.0, 1.0);

	_explainLabel = new UILabel();
	_explainLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::LEFT);
	_explainLabel->pos(8, 55);
	_explainLabel->text("TIME REMAINING");
	_explainLabel->parent(this);

	_timeLabel = new UILabel();
	_timeLabel->init("resources/fonts/sui_generis_free.ttf", 36, UILabel::CENTER);
	_timeLabel->pos(110, 8);
	printTimeToBuffer();
	_timeLabel->text(_timeBuf);
	_timeLabel->shadow(true);
	_timeLabel->parent(this);
}

void UICountdown::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class

	// TODO: eventually fetch the actual score from the game state here
	static int accum = 0;
	accum += ms;
	_seconds = accum / 1000;
	_seconds = _seconds % 600;
	printTimeToBuffer();
	_timeLabel->text(_timeBuf);

	// update our children
	updateChildren(ms);
}

void UICountdown::draw()
{
	// DON'T draw the base class
	//UIControl::draw();

	// draw this class
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor4f(_bgr, _bgg, _bgb, _bga);
			glVertex2f(_parent->x() + _x, _parent->y() + _y);
			glVertex2f(_parent->x() + _x + _w - 50, _parent->y() + _y);
			glVertex2f(_parent->x() + _x + _w - 50, _parent->y() + _y + _h);
			glVertex2f(_parent->x() + _x, _parent->y() + _y + _h);
		glEnd();
		glBegin(GL_QUADS);
			glColor4f(_bgr, _bgg, _bgb, _bga);
			glVertex2f(_parent->x() + _x + _w - 50, _parent->y() + _y);
			glVertex2f(_parent->x() + _x + _w, _parent->y() + _y);
			glVertex2f(_parent->x() + _x + _w, _parent->y() + _y + _h - 50);
			glVertex2f(_parent->x() + _x + _w - 50, _parent->y() + _y + _h - 50);
		glEnd();
		glBegin(GL_TRIANGLES);
			glColor4f(_bgr, _bgg, _bgb, _bga);
			glVertex2f(_parent->x() + _x + _w - 50, _parent->y() + _y + _h - 50);
			glVertex2f(_parent->x() + _x + _w, _parent->y() + _y + _h - 50);
			glVertex2f(_parent->x() + _x + _w - 50, _parent->y() + _y + _h);
		glEnd();
	glPopMatrix();

	// draw our children
	drawChildren();
}

void UICountdown::printTimeToBuffer()
{
	int minutes = _seconds / 60;
	int seconds = _seconds % 60;
	if (seconds < 0) seconds *= -1;
	snprintf(_timeBuf, 20, "%d:%02d", minutes, seconds);
}
