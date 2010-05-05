#ifndef _GAMEUI_H_
#define _GAMEUI_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <vector>

#include "../system/global.h"
#include "../helper/luahelper.h"
#include "UIControl.h"

class GameUI {
public:
    GameUI();
    ~GameUI();

    virtual void init(const char *script);
    virtual void update(int ms) = 0;
    virtual void draw();

    // property accessors
    bool dimmed() const { return _dimmed; }
    void dimmed(bool dimmed) { _dimmed = dimmed; }
    bool show() const { return _show; }
    void show(bool show) { _show = show; }

    // define UI constants here
    static const int NONE;
    static const int BUILD;
    static const int CARNAGE;
    static const int MENU;
    static const int BLANK;

protected:
    std::vector<UIControl *> _controls;
    lua_State *_L;
    bool _dimmed;
    bool _show;
};

#endif
