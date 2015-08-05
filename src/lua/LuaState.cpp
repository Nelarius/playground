#include "lua/LuaState.h"
#include "lua/LuaError.h"
#include "utils/File.h"
#include "utils/Assert.h"
#include "utils/Log.h"

extern "C" {
    #include <lualib.h>
    #include <lauxlib.h>
}

using pg::LuaState;

LuaState::LuaState( bool loadMinimal )
:   luaState_( luaL_newstate() ),
    refCount_( nullptr ) {
    refCount_ = new unsigned;
    *refCount_ = 1u;
    if ( loadMinimal ) {
        loadMinimal_();
    } else {
        luaL_openlibs( luaState_ );
    }
}

LuaState::LuaState( const LuaState& other )
:   luaState_( other.luaState_ ),
    refCount_( other.refCount_ ) {
    retain_();
}

LuaState& LuaState::operator=( const LuaState& rhs ) {
    release_();
    luaState_ = rhs.luaState_;
    refCount_ = rhs.refCount_;
    retain_();
    return *this;
}

LuaState::LuaState( LuaState&& other )
:   luaState_( other.luaState_ ),
    refCount_( other.refCount_ ) {
    other.luaState_ = nullptr;
    other.refCount_ = nullptr;
}

LuaState& LuaState::operator=( LuaState&& rhs ) {
    release_();
    luaState_ = rhs.luaState_;
    refCount_ = rhs.refCount_;
    rhs.luaState_ = nullptr;
    rhs.refCount_ = nullptr;
    return *this;
}

LuaState::~LuaState() {
    release_();
}

void LuaState::execute( const std::string& file ) {
    if ( !ce::FileExists( file ) ) {
        LOG(ce::LogLevel::Error) << "LuaState::execute> No such file " << file;
        return;
    }
    luaL_loadfile( luaState_, file.c_str() );
    int res = lua_pcall( luaState_, 0, LUA_MULTRET, 0 );
    if ( res ) {
        LOG( ce::LogLevel::Error ) << pg::CodeToString( res ) << " " <<  lua_tostring( luaState_, -1 );
        // stack trace should go here somehow
    }
}

lua_State* LuaState::get() const {
    return luaState_;
}

void LuaState::loadMinimal_() {
    lua_pushcfunction( luaState_, luaopen_base );
    lua_pushliteral( luaState_, "" );
    lua_call( luaState_, 1, 0 );    //there shouldn't be anything on the stack now

    lua_pushcfunction( luaState_, luaopen_table );
    lua_pushliteral( luaState_, LUA_TABLIBNAME );
    lua_call( luaState_, 1, 0 );

    lua_pushcfunction( luaState_, luaopen_string );
    lua_pushliteral( luaState_, LUA_STRLIBNAME );
    lua_call( luaState_, 1, 0 );

    lua_pushcfunction( luaState_, luaopen_math );
    lua_pushliteral( luaState_, LUA_MATHLIBNAME );
    lua_call( luaState_, 1, 0 );
}

void LuaState::retain_() {
    ASSERT( refCount_, "LuaState::retain> invalid reference count!" );
    *refCount_ += 1u;
}

void LuaState::release_() {
    if ( refCount_ ) {
        *refCount_ -= 1u;
        if ( *refCount_ == 0u ) {
            lua_close( luaState_ );
            delete refCount_;
            refCount_ = nullptr;
        }
    }
}


