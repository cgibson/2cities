#ifndef _GAMEUI_H_
#define _GAMEUI_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdlib.h>

#include "UIWindow.h"

class GameUI {
public:
	GameUI();
    virtual ~GameUI();

	virtual void init(float r, float g, float b, float a);
	virtual void update(int ms);
	virtual void draw();

	// property accessors
	UIWindow *window() const { return _window; }

protected:
    UIWindow *_window;
};

#endif
