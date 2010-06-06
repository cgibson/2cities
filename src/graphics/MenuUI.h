#ifndef _MENUUI_H_
#define _MENUUI_H_

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

class MenuUI : public GameUI {
public:
	MenuUI();
    ~MenuUI();

	void init();
	void update(int ms);
	void draw();

	void keyDown(int key, bool special);
	void mouseDown(int button);
	void mouseWheel(int direction);

private:
	UILabel *_twoLabel;
	UILabel *_citiesLabel;
	UILabel *_hostGame;
	UILabel *_joinGame;
	UILabel *_exitGame;
	UILabel *_ipLabel;
	UILabel *_ipInput;
	int _currentChoice;
	float _velocity;
	float _offset;
	bool _choiceMade;
	float _ipAlpha;
	float _ipAlphaVel;

	void prevChoice();
	void nextChoice();

	void startHosting();
	void startJoining();

	void addIPchar(char c);
	void removeIPchar();

	static const float SPRING;
	static const float FRICTION;
};

#endif


