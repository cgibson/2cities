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
#include "../graphics/ShockwaveEffect.h"
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
	UILabel *_cxnFailed;
	int _currentChoice;
	float _velocity;
	float _offset;
	bool _choiceMade;
	float _ipAlpha;
	float _ipAlphaVel;
	bool _intro;
	int _introStart;
	float _introPos;
	bool _firedShockwave;
	int _cxnFailedStart;

	void prevChoice();
	void nextChoice();

	void startHosting();
	void startJoining();

	void addIPchar(char c);
	void removeIPchar();

	float introBounce(float t);

	static const float SPRING;
	static const float FRICTION;
	static const int INTRO_DURATION_MS;
	static const int CXN_FAILED_DURATION;
};

#endif


