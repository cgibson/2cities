#ifndef _LUAHELPER_H_
#define _LUAHELPER_H_

#include <stdlib.h>
#include "lua.hpp"

namespace lua
{
    // pass the filename of the lua script to load, along with a function
    // pointer to a function that makes all your lua_register() calls to
    // connect lua calls to your c code
    lua_State *setup(const char *scriptName, void (*registerFuncs)(lua_State *L));
}

#endif
