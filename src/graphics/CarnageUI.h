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
#include <string.h>

#include "GameUI.h"
#include "UILabel.h"
#include "UIIcon.h"
#include "UIProgress.h"
#include "UIScoreDisplay.h"
#include "UICountdown.h"

class CarnageUI : public GameUI {
public:
	CarnageUI();
    ~CarnageUI();

	void init();
	void update(int ms);
	void draw();

private:
	UIIcon *_reticle;
	UIScoreDisplay *_redScore;
	UIScoreDisplay *_blueScore;
	UILabel *_redDestructLabel;
	UILabel *_blueDestructLabel;
	UIProgress *_redDestructProgress;
	UIProgress *_blueDestructProgress;
	char _redDestructBuf[10];
	char _blueDestructBuf[10];
	UICountdown *_countdown;
};

#endif

