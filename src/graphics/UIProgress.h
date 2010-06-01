#ifndef _UIPROGRESS_H_
#define _UIPROGRESS_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "UIControl.h"

class UIProgress : public UIControl {
public:
	typedef enum E_PROGRESSDIR
	{
		LEFT,
		RIGHT
	} ProgressDir;

	UIProgress();
	~UIProgress();

	void init(ProgressDir dir);
	void update(int ms);
	void draw();

	// property accessors
	float fgr() const { return _fgr; }
    float fgg() const { return _fgg; }
    float fgb() const { return _fgb; }
    float fga() const { return _fga; }
    void fgclr(float r, float g, float b, float a) { _fgr = r; _fgg = g; _fgb = b; _fga = a; }
	int percent() const { return _percent; }
	void percent(int percent) { _percent = (percent < 0) ? 0 : percent; _percent = (_percent > 100) ? 100 : _percent; }

private:
	float _fgr;
	float _fgg;
	float _fgb;
	float _fga;
	ProgressDir _dir;
	int _percent;
};

#endif
