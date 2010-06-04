#ifndef _WAITINGUI_H_
#define _WAITINGUI_H_

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
#include "GameUI.h"
#include "UILabel.h"
#include "UIIcon.h"

class WaitingUI : public GameUI {
public:
	WaitingUI();
    ~WaitingUI();

	void init();
	void update(int ms);
	void draw();

private:
	UILabel *_waitingLabel;
	UIIcon *_waitingIcon;
};

#endif


