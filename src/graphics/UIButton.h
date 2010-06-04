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

	void init(void (*click_callback)());
	void update(int ms);
	void draw();

	// property accessors
	UILabel *text() const { return _text; }
	UIIcon *icon() const { return _icon; }
	void mouseOverClr(float r, float g, float b, float a) {_mor = r; _mog = g; _mob = b; _moa = a; }
	void mouseDownClr(float r, float g, float b, float a) {_mdr = r; _mdg = g; _mdb = b; _mda = a; }

private:
	typedef enum E_BUTTONSTATE {
		OUT,
		OVER,
		DOWN
	} ButtonState;

	UILabel *_text;
	UIIcon *_icon;
	float _mor, _mog, _mob, _moa;
	float _mdr, _mdg, _mdb, _mda;
	ButtonState _state;
	void (*_click_callback)();
};

#endif
