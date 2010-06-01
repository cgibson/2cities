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
#include <vector>

class UIControl {
public:
    UIControl();
    virtual ~UIControl();

    virtual void init(int x, int y, int w, int h, float bgr, float bgg, float bgb, float bga);
    virtual void update(int ms);
    virtual void draw();

    // property accessors
    int x() const { return _x; }
    int y() const { return _y; }
    void pos(int x, int y) { _x = x; _y = y; }
    int width() const { return _w; }
    int height() const { return _h; }
    void size(int width, int height) { _w = width; _h = height; }
    float bgr() const { return _bgr; }
    float bgg() const { return _bgg; }
    float bgb() const { return _bgb; }
    float bga() const { return _bga; }
    void bgclr(float r, float g, float b, float a) { _bgr = r; _bgg = g; _bgb = b; _bga = a; }
    UIControl *parent() const { return _parent; }
    void parent(UIControl *parent) { _parent = parent; parent->addChild(this); }

protected:
    int _x, _y, _w, _h;
    float _bgr, _bgg, _bgb, _bga;
    UIControl *_parent;
	std::vector<UIControl *> _children;
	void addChild(UIControl *child) { _children.push_back(child); }
	void updateChildren(int ms);
	void drawChildren();
};

#endif
