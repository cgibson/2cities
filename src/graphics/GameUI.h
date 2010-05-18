#ifndef _GAMEUI_H_
#define _GAMEUI_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "../system/global.h"
#include "../helper/luahelper.h"
#include "UIControl.h"
#include "UILabel.h"

#define UI_SCRIPT_DIR "resources/ui/"

class GameUI {
public:
	GameUI(const char *script);
    ~GameUI();

	void init();
	void update(int ms);
	void draw();

	void addControl(UIControl *control);
	void unload();
	void reset();

    // property accessors
	const char *script() const { return _script; }
	lua_State *luastate() const { return _L; }
    bool dimmed() const { return _dimmed; }
    void dimmed(bool dimmed) { _dimmed = dimmed; }
    bool done() const { return _done; }
    void done(bool done) { _done = done; }

    static void registerLuaFuncs(lua_State *L);
    static int ui_add(lua_State *L);
    static int ui_close(lua_State *L);

private:
    std::vector<UIControl *> _controls;
    lua_State *_L;
    bool _dimmed;
	char *_script;
	bool _done;

	static GameUI *getUIFromLuaState(lua_State *L);
};

#endif
