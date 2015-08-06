
#pragma once

#include "lua/LuaBridge.h"

namespace pg {

void BindAll( lua_State* );
void BindVector( lua_State* );
void BindComponent( lua_State* );
void BindEntity( lua_State* );

}

