#ifndef _UICOUNTDOWN_H_
#define _UICOUNTDOWN_H_

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
#include "UIControl.h"
#include "UILabel.h"

class UICountdown : public UIControl {
public:
	UICountdown();
	~UICountdown();

	void init();
	void update(int ms);
	void draw();

private:
	UILabel *_explainLabel;
	UILabel *_timeLabel;
	int _seconds;
	char _timeBuf[20];
	void printTimeToBuffer();
};

#endif