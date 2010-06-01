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
	void tintclr(float r, float g, float b) { _tintr = r; _tintg = g, _tintb = b; }
	void tint(bool tint) { _tint = tint; }
	float rotAngle() const { return _rotAngle; }
	void rotAngle(float rotAngle) { _rotAngle = rotAngle; }

private:
	GLuint _image_tex;
	GLuint _alpha_tex;
	bool _alpha;
	bool _tint;
	float _tintr, _tintg, _tintb;
	int _naturalWidth;
	int _naturalHeight;
	float _rotAngle;
};

#endif
