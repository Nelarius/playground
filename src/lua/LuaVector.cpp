
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include "lua/CreateBindings.h"
#include "linalg/Vector.h"

#include <iostream>

void ce::BindVector( lua_State* l ) {
    luabridge::getGlobalNamespace( l )
        .beginClass<ce::Vector3f>("Vector3f")
            .addConstructor<void(*)( float, float, float )>()
            .addData( "x", &ce::Vector3f::x )
            .addData( "y", &ce::Vector3f::y )
            .addData( "z", &ce::Vector3f::z )
            .addData( "r", &ce::Vector3f::r )
            .addData( "g", &ce::Vector3f::g )
            .addData( "b", &ce::Vector3f::b )
            .addFunction( "l2norm", &ce::Vector3f::l2norm )
            .addFunction( "normalize", &ce::Vector3f::normalize )
            .addFunction( "length", &ce::Vector3f::length )
            .addFunction( "squaredLength", &ce::Vector3f::squaredLength )
        .endClass()
        
        .beginClass<ce::Vector4f>("Vector4f")
            .addConstructor<void(*)( float, float, float, float )>()
            .addData( "x", &ce::Vector4f::x )
            .addData( "y", &ce::Vector4f::y )
            .addData( "z", &ce::Vector4f::z )
            .addData( "w", &ce::Vector4f::w )
            .addData( "r", &ce::Vector4f::r )
            .addData( "g", &ce::Vector4f::g )
            .addData( "b", &ce::Vector4f::b )
            .addData( "a", &ce::Vector4f::a )
            .addFunction( "l2norm", &ce::Vector4f::l2norm )
            .addFunction( "normalize", &ce::Vector4f::normalize )
            .addFunction( "length", &ce::Vector4f::length )
            .addFunction( "squaredLength", &ce::Vector4f::squaredLength )
        .endClass();
}
