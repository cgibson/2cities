#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../system/global.h"
#include "../system/io.h"
#include "UIControl.h"
#include "UILabel.h"
#include "UIIcon.h"

class UIButton : public UIControl {
public:
	UIButton();
	~UIButton();

	void init();
	void update(int ms);
	void draw();

	// property accessors
	UILabel *text() const { return _text; }
	UIIcon *icon() const { return _icon; }

private:
	UILabel *_text;
	UIIcon *_icon;
};

#endif
