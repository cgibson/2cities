#include "GameUI.h"
#include "graphics.h"
#include "hud.h"

GameUI::GameUI(const char *script)
{
    _dimmed = false;
    _done = false;

    _script = (char *)malloc(sizeof(char) * (strlen(script) + 1));
    memcpy(_script, script, strlen(script));
    _script[strlen(script)] = '\0';
}

GameUI::~GameUI()
{
    if (_script != NULL) free(_script);
    _controls.clear();
}

void GameUI::init()
{
    // get the full path to the script
    int dir_len = strlen(UI_SCRIPT_DIR);
    int script_len = strlen(_script);
    char *filename = (char *)malloc(sizeof(char) * (dir_len + script_len + 1));
    memcpy(filename, UI_SCRIPT_DIR, dir_len);
    memcpy(filename + dir_len, _script, script_len);
    filename[dir_len + script_len] = '\0';

    // load the lua script
    _L = lua::setup(filename, GameUI::registerLuaFuncs);

    // all done with the filename
    free(filename);

    // parse out window (game ui wide) properties
    lua_getglobal(_L, "window");
		if (!lua_istable(_L, -1)) luaL_error(_L, "Global variable 'window' is not a table!");
		lua_getfield(_L, -1, "dimmed");
			if (!lua_isboolean(_L, -1)) luaL_error(_L, "window.dimmed should be a boolean value!");
			_dimmed = lua_toboolean(_L, -1);
		lua_pop(_L, 1);
	lua_pop(_L, 1);

	// call the init function to hook up ui components
	lua_getglobal(_L, "init");
	if (!lua_isnil(_L, -1))
	{
		// function exists, call it
		lua_call(_L, 0, 0); // 0 args, 0 retvals
	}
	else
	{
		// pop nil off the stack
		lua_pop(_L, 1);
	}
}

void GameUI::update(int ms)
{
	// fire off the lua script's update function (if defined)
	lua_getglobal(_L, "update");
	if (!lua_isnil(_L, -1))
	{
		// function exists, call it
		lua_pushnumber(_L, ms);
		lua_call(_L, 1, 0); // 1 arg, 0 retvals
	}
	else
	{
		// pop nil off the stack
		lua_pop(_L, 1);
	}

	// update each of the control's values from the script
	for (int i = 0; i < (int)_controls.size(); i++)
	{
		_controls[i]->update(_L, ms);
	}
}

void GameUI::draw()
{
    if (_dimmed)
    {
        // draw a half transparent quad across the whole screen
        glBegin(GL_QUADS);
            glColor4f(0.0, 0.0, 0.0, 0.5);
            glVertex2i(0, 0);
            glVertex2i(global::width, 0);
            glVertex2i(global::width, global::height);
            glVertex2i(0, global::height);
        glEnd();
    }

    // draw all controls
    for (int i = 0; i < (int)_controls.size(); i++)
    {
		_controls[i]->draw();
	}
}

void GameUI::addControl(UIControl *control)
{
	_controls.push_back(control);
}

void GameUI::unload()
{
	// set the lua ui_unload flag
	lua_pushboolean(_L, true);
	lua_setglobal(_L, "ui_unload");
}

void GameUI::reset()
{
	_done = false;
	lua_pushboolean(_L, false);
	lua_setglobal(_L, "ui_unload");
}

void GameUI::registerLuaFuncs(lua_State *L)
{
	lua_register(L, "ui_add", GameUI::ui_add);
	lua_register(L, "ui_close", GameUI::ui_close);
}

int GameUI::ui_add(lua_State *L)
{
	GameUI *ui = getUIFromLuaState(L);

	// check the arguments
	int argc = lua_gettop(L);
	if (argc != 2) luaL_error(L, "Expected 2 arguments (type and varname) to ui_add!");
	if (!lua_isstring(L, -2)) luaL_error(L, "Type argument to ui_add should be a string!");
	if (!lua_isstring(L, -1)) luaL_error(L, "Varname argument to ui_add should be a string!");

	// grab the type and name arguments
	const char *type = lua_tostring(L, -2);
	const char *name = lua_tostring(L, -1);
	lua_pop(L, 2);

	// construct a new ui control based on the type
	UIControl *control;
	if (strcmp(type, "label") == 0)
	{
		control = new UILabel(name);
	}
	else
	{
		luaL_error(L, "Unknown control type '%s'", type);
	}
	control->init(L);

	// initialize and add the control
	ui->addControl(control);

	// DEBUG
	gfx::hud.console.info("Added %s control '%s' to %s.", type, control->name(), ui->script());

	return 0;
}

int GameUI::ui_close(lua_State *L)
{
	GameUI *ui = getUIFromLuaState(L);
	ui->done(true);

	return 0;
}

GameUI *GameUI::getUIFromLuaState(lua_State *L)
{
	// find which ui we're talking about by comparing lua state pointers
	int ui_num = -1;
	for (int i = 0; i < gfx::hud.numUIs(); i++)
	{
		if (gfx::hud.getUI(i)->luastate() == L)
		{
			// found it!
			ui_num = i;
			break;
		}
	}

	if (ui_num < 0) return NULL; // this should never happen (famous last words)
	return gfx::hud.getUI(ui_num);
}
