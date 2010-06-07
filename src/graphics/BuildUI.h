#ifndef _BUILDUI_H_
#define _BUILDUI_H_

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
#include "UIProgress.h"
#include "UIScoreDisplay.h"
#include "UIWheelChooser.h"
#include "UIButton.h"

class BuildUI : public GameUI {
public:
	BuildUI();
    ~BuildUI();

	void init();
	void update(int ms);
	void draw();

	void ready();

	void keyDown(int key, bool special);
	void mouseWheel(int direction);

	static void readyClick();

private:
	UIScoreDisplay *_redResources;
	UIScoreDisplay *_blueResources;
	UILabel *_redFullLabel;
	UILabel *_blueFullLabel;
	UIProgress *_redFullProgress;
	UIProgress *_blueFullProgress;
	UIWheelChooser *_typeSelect;
	UILabel *_weakMult;
	UILabel *_strongMult;
	UIButton *_readyButton;
	UILabel *_otherPlayerReady;
	bool _parentedOtherPlayerReady;
	char _redFullBuf[10];
	char _blueFullBuf[10];
};

#endif

