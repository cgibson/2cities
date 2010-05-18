#include "UILabel.h"

UILabel::UILabel(const char *name)
	: UIControl(name)
{
	// nothing at the moment
}

UILabel::~UILabel()
{
	// nothing at the moment
}

void UILabel::init(lua_State *L)
{
	// initialize common properties
	UIControl::init(L);

	// initalize label-specific properties

}

void UILabel::update(lua_State *L, int ms)
{
	// update common properties
	UIControl::update(L, ms);

	// update label-specific properties

}

void UILabel::draw()
{
	glBegin(GL_QUADS);
		glColor4f(_bgr, _bgg, _bgb, _bga);
		glVertex2i(_x, _y);
		glVertex2i(_x + _width, _y);
		glVertex2i(_x + _width, _y + _height);
		glVertex2i(_x, _y + _height);
	glEnd();
}
