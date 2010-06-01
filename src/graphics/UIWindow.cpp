#include "UIWindow.h"

UIWindow::UIWindow()
	: UIControl()
{
	// nothing at the moment
}

UIWindow::~UIWindow()
{
	// nothing at the moment
}

void UIWindow::init(float r, float g, float b, float a)
{
	UIControl::init(0, 0, global::width, global::height, r, g, b, a);
	_parent = NULL;
}

void UIWindow::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class
	_w = global::width;
	_h = global::height;

	// update our children
	updateChildren(ms);
}

void UIWindow::draw()
{
	// draw the base class
	UIControl::draw();

	// draw this class

	// draw our children
	drawChildren();
}
