
#pragma once

#include "utils/Log.h"
extern "C" {
    #include <lua.h>
}

namespace ce {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
inline std::string CodeToString( int code ) {
    switch( code ) {
        case LUA_OK:        return "LUA_OK";
        case LUA_ERRRUN:    return "LUA_ERRRUN";
        case LUA_ERRMEM:    return "LUA_ERRMEM";
        case LUA_ERRERR:    return "LUA_ERRERR";
        case LUA_ERRGCMM:   return "LUA_ERRGCMM";
        default: 
            LOG(ce::LogLevel::Error) << "CodeToString: no such error code " << code;
            return "";
    }
}
#pragma GCC diagnostic pop

}