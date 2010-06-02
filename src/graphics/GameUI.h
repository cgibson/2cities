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

	// default event handlers (does nothing, override to change)
	virtual void keyDown(int key, bool special);
	virtual void keyUp(int key, bool special);
	virtual void mouseDown(int button);
	virtual void mouseUp(int button);
	virtual void mouseWheel(int direction);

	// static event handler wrappers
	static void key_down_handler(int key, bool special);
	static void key_up_handler(int key, bool special);
	static void mouse_down_handler(int button);
	static void mouse_up_handler(int button);
	static void mouse_wheel_handler(int direction);

protected:
    UIWindow *_window;
};

#endif
