
#pragma once

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

namespace pg {
    
void BindAll( lua_State* );
void BindVector( lua_State* );

}

