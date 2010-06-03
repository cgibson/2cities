#ifndef _UISCOREDISPLAY_H_
#define _UISCOREDISPLAY_H_

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

class UIScoreDisplay : public UIControl {
public:
	typedef enum E_SCOREDISPLAY
	{
		LEFT_PLAYER,
		RIGHT_PLAYER
	} ScoreDisplay;

	typedef enum E_SCORETYPE
	{
		SCORE,
		RESOURCES
	} ScoreType;

	UIScoreDisplay();
	~UIScoreDisplay();

	void init(ScoreDisplay player, ScoreType type);
	void update(int ms);
	void draw();

private:
	ScoreDisplay _player;
	ScoreType _type;
	UILabel *_explainLabel;
	UILabel *_scoreLabel;
	int _score;
	char _scoreBuf[10];
	void printScoreToBuffer();
};

#endif
