#ifndef _CARNAGEUI_H_
#define _CARNAGEUI_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "GameUI.h"
#include "UILabel.h"
#include "UIIcon.h"
#include "UIProgress.h"

class CarnageUI : public GameUI {
public:
	CarnageUI();
    ~CarnageUI();

	void init();
	void update(int ms);
	void draw();

private:
	UILabel *_testLabel;
	UIIcon *_testIcon;
	UIProgress *_testProgress;
};

#endif

