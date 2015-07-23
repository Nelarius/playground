
#pragma once

extern "C" {
    #include <lua.h>
}
#include <string>

namespace ce {

class LuaState {
    public:
        explicit LuaState( bool loadMinimal = true );
        LuaState()  = delete;
        LuaState( const LuaState& );
        LuaState( LuaState&& );
        LuaState& operator=( const LuaState& );
        LuaState& operator=( LuaState&& );
        ~LuaState();
        
        void execute( const std::string& file );
        
        inline operator lua_State*() {
            return luaState_;
        }
        
        lua_State* get();
    
    private:
        void retain_();
        void release_();
        void loadMinimal_();
        
        lua_State*  luaState_{ nullptr };
        unsigned*   refCount_{ nullptr };
};
    
}   // ce
