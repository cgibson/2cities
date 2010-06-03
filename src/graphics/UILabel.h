#ifndef _UILABEL_H_
#define _UILABEL_H_

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

#include "UIControl.h"
#include "OGLFT.h"

class UILabel : public UIControl {
public:
	typedef enum E_JUSTIFICATION
	{
		LEFT,
		CENTER,
		RIGHT
	} TextJustify;

	UILabel();
	~UILabel();

	void init(const char *font, int size, TextJustify justify);
	void update(int ms);
	void draw();

	// property accessors
	float fgr() const { return _fgr; }
    float fgg() const { return _fgg; }
    float fgb() const { return _fgb; }
    float fga() const { return _fga; }
    void fgclr(float r, float g, float b, float a) { _fgr = r; _fgg = g; _fgb = b; _fga = a; }
    bool shadow() const { return _shadow; }
    void shadow(bool shadow) { _shadow = shadow; }
    const char *text() { return _text; }
    void text(const char *text) {
		if (_text != NULL) free(_text);
		_text = (char *)malloc(sizeof(char) * (strlen(text) + 1));
		memcpy(_text, text, strlen(text));
		_text[strlen(text)] = '\0';
	}
	int fontSize() const { return _fontSize; }

private:
	float _fgr;
	float _fgg;
	float _fgb;
	float _fga;
	bool _shadow;
	OGLFT::Translucent *_font;
	char *_text;
	int _fontSize;

	static const int DROP_X;
	static const int DROP_Y;
};

#endif
