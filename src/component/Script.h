
#pragma once

#include "lua/LuaState.h"
#include "lua/LuaBridge.h"

namespace pg {
namespace component {

struct Script {
    LuaState state;
};


}
}
