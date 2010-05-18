#include "UIControl.h"

UIControl::UIControl(const char *name)
{
	_name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    memcpy(_name, name, strlen(name));
    _name[strlen(name)] = '\0';

    _x = _y = 0;
    _width = _height = 100;
    _bgr = _bgg = _bgb = 0.0;
    _fgr = _fgg = _fgb = 1.0;
    _bga = _fga = 1.0;
}

UIControl::~UIControl()
{
    if (_name != NULL) free(_name);
}

void UIControl::init(lua_State *L)
{
	getLuaVals(L);
}

void UIControl::update(lua_State *L, int ms)
{
	getLuaVals(L);
}

void UIControl::getLuaVals(lua_State *L)
{
	lua_getglobal(L, _name);
		if (!lua_istable(L, -1)) luaL_error(L, "Control '%s' should be a table!", _name);
		lua_getfield(L, -1, "pos");
			if (!lua_istable(L, -1)) luaL_error(L, "Pos element of '%s' should be a table!", _name);
			lua_rawgeti(L, -1, 1);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Position x of '%s' should be a number!", _name);
				_x = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_rawgeti(L, -1, 2);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Position y of '%s' should be a number!", _name);
				_y = lua_tonumber(L, -1);
			lua_pop(L, 1);
		lua_pop(L, 1);
		lua_getfield(L, -1, "size");
			if (!lua_istable(L, -1)) luaL_error(L, "Size element of '%s' should be a table!", _name);
			lua_rawgeti(L, -1, 1);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Size width of '%s' should be a number!", _name);
				_width = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_rawgeti(L, -1, 2);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Size height of '%s' should be a number!", _name);
				_height = lua_tonumber(L, -1);
			lua_pop(L, 1);
		lua_pop(L, 1);
		lua_getfield(L, -1, "bgclr");
			if (!lua_istable(L, -1)) luaL_error(L, "Background color element of '%s' should be a table!", _name);
			lua_rawgeti(L, -1, 1);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Background color red of '%s' should be a number!", _name);
				_bgr = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_rawgeti(L, -1, 2);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Background color green of '%s' should be a number!", _name);
				_bgg = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_rawgeti(L, -1, 3);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Background color blue of '%s' should be a number!", _name);
				_bgb = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_rawgeti(L, -1, 4);
				if (!lua_isnumber(L, -1)) luaL_error(L, "Background color alpha of '%s' should be a number!", _name);
				_bga = lua_tonumber(L, -1);
			lua_pop(L, 1);
		lua_pop(L, 1);
	lua_pop(L, 1);
}
