#ifndef _RESULTSUI_H_
#define _RESULTSUI_H_

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
#include "../graphics/ShockwaveEffect.h"
#include "GameUI.h"
#include "UILabel.h"
#include "UIIcon.h"
#include "UIButton.h"

class ResultsUI : public GameUI {
public:
	ResultsUI();
    ~ResultsUI();

	void init();
	void update(int ms);
	void draw();
	
	void rematch();
	void menu();

	static void rematchClick();
	static void menuClick();

private:
	UILabel *_winLabel;
	UILabel *_redScore;
	UILabel *_blueScore;
	UIButton *_rematchButton;
	UIButton *_menuButton;
	UILabel *_gauntletLabel;
	char _redBuf[10];
	char _blueBuf[10];
};

#endif
