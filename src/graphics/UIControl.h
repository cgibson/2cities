#ifndef _UICONTROL_H_
#define _UICONTROL_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "../helper/luahelper.h"

class UIControl {
public:
    UIControl();
    ~UIControl();

    virtual void init() {};
    virtual void update(int ms) {};
    virtual void draw() {};

    // property accessors
    int x() const { return _x; }
    int y() const { return _y; }
    void position(int x, int y) { _x = x; _y = y; }
    int width() const { return _width; }
    int height() const { return _height; }
    void dimensions(int width, int height) { _width = width; _height = height; }
    float r() const { return _r; }
    float g() const { return _g; }
    float b() const { return _b; }
    float a() const { return _a; }
    void color(float r, float g, float b, float a) { _r = r; _g = g; _b = b; _a = a; }

protected:
    int _x, _y, _width, _height;
    float _r, _g, _b, _a;
};

#endif
