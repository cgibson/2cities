#ifndef _UIICON_H_
#define _UIICON_H_

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

#include "../helper/texloader.h"
#include "UIControl.h"

class UIIcon : public UIControl {
public:
	UIIcon();
	~UIIcon();

	void init(const char *image, const char *alpha = NULL);
	void update(int ms);
	void draw();

	// property accessors
	int naturalWidth() const { return _naturalWidth; }
	int naturalHeight() const { return _naturalHeight; }

private:
	GLuint _image_tex;
	GLuint _alpha_tex;
	bool _alpha;
	int _naturalWidth;
	int _naturalHeight;
};

#endif
