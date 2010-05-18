#ifndef _UICONTROL_H_
#define _UICONTROL_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "../helper/luahelper.h"

class UIControl {
public:
    UIControl(const char *name);
    virtual ~UIControl();

    virtual void init(lua_State *L);
    virtual void update(lua_State *L, int ms);
    virtual void draw() = 0;

    // property accessors
    const char *name() const { return _name; }
    int x() const { return _x; }
    int y() const { return _y; }
    void pos(int x, int y) { _x = x; _y = y; }
    int width() const { return _width; }
    int height() const { return _height; }
    void size(int width, int height) { _width = width; _height = height; }
    float bgr() const { return _bgr; }
    float bgg() const { return _bgg; }
    float bgb() const { return _bgb; }
    float bga() const { return _bga; }
    void bgclr(float r, float g, float b, float a) { _bgr = r; _bgg = g; _bgb = b; _bga = a; }
    float fgr() const { return _fgr; }
    float fgg() const { return _fgg; }
    float fgb() const { return _fgb; }
    float fga() const { return _fga; }
    void fgclr(float r, float g, float b, float a) { _fgr = r; _fgg = g; _fgb = b; _fga = a; }

protected:
	char *_name;
    int _x, _y, _width, _height;
    float _bgr, _bgg, _bgb, _bga;
    float _fgr, _fgg, _fgb, _fga;

private:
	void getLuaVals(lua_State *L);
};

#endif
