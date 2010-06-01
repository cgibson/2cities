#ifndef _UIWINDOW_H_
#define _UIWINDOW_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "../system/global.h"
#include "UIControl.h"

class UIWindow : public UIControl {
public:
	UIWindow();
	~UIWindow();

	void init(float r, float g, float b, float a);
	void update(int ms);
	void draw();
};

#endif
