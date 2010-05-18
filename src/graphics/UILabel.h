#ifndef _UILABEL_H_
#define _UILABEL_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "UIControl.h"

class UILabel : public UIControl {
public:
	UILabel(const  char *name);
	~UILabel();

	void init(lua_State *L);
	void update(lua_State *L, int ms);
	void draw();

private:

};

#endif
