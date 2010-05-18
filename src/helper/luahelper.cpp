#include "luahelper.h"

namespace lua
{
    lua_State *setup(const char *scriptName, void (*registerFuncs)(lua_State *L))
    {
        // create new interpreter state
        lua_State *L = luaL_newstate();

        // load the standard libraries
        luaL_openlibs(L);

        // register our C function hooks
        registerFuncs(L);

        // run the interpreter on the script
        if (luaL_dofile(L, scriptName))
        {
            fprintf(stderr, "%s\n", lua_tostring(L, -1));
            exit(EXIT_FAILURE);
        }

        return L;
    }
}
