#include "lua/BindLua.h"
#include "math/Vector.h"
#include <LuaBridge/LuaBridge.h>

void ce::BindAll( lua_State* l ) {
    ce::BindVector( l );
}

void ce::BindVector( lua_State* l ) {
    luabridge::getGlobalNamespace( l )
        .beginNamespace( "pg" )
            .beginClass<ce::math::Vector2f>( "Vector2f" )
                .addConstructor<void(*)( float, float )>()
                .addData( "x", &ce::math::Vector2f::x )
                .addData( "y", &ce::math::Vector2f::y )
                .addData( "r", &ce::math::Vector2f::r )
                .addData( "g", &ce::math::Vector2f::g )
                .addFunction( "l2norm", &ce::math::Vector2f::l2norm )
                .addFunction( "normalize", &ce::math::Vector2f::normalize )
                .addFunction( "length", &ce::math::Vector2f::length )
                .addFunction( "squaredLength", &ce::math::Vector2f::squaredLength )
            .endClass()
            .beginClass<ce::math::Vector3f>( "Vector3f" )
                .addConstructor<void(*)( float, float, float )>()
                .addData( "x", &ce::math::Vector3f::x )
                .addData( "y", &ce::math::Vector3f::y )
                .addData( "z", &ce::math::Vector3f::z )
                .addData( "r", &ce::math::Vector3f::r )
                .addData( "g", &ce::math::Vector3f::g )
                .addData( "b", &ce::math::Vector3f::b )
                .addFunction( "l2norm", &ce::math::Vector3f::l2norm )
                .addFunction( "normalize", &ce::math::Vector3f::normalize )
                .addFunction( "length", &ce::math::Vector3f::length )
                .addFunction( "squaredLength", &ce::math::Vector3f::squaredLength )
                .addFunction( "cross", &ce::math::Vector3f::cross )
            .endClass()
            .beginClass<ce::math::Vector4f>( "Vector4f" )
                .addConstructor<void(*)( float, float, float, float )>()
                .addData( "x", &ce::math::Vector4f::x )
                .addData( "y", &ce::math::Vector4f::y )
                .addData( "z", &ce::math::Vector4f::z )
                .addData( "w", &ce::math::Vector4f::w )
                .addData( "r", &ce::math::Vector4f::r )
                .addData( "g", &ce::math::Vector4f::g )
                .addData( "b", &ce::math::Vector4f::b )
                .addData( "a", &ce::math::Vector4f::a )
                .addFunction( "l2norm", &ce::math::Vector4f::l2norm )
                .addFunction( "normalize", &ce::math::Vector4f::normalize )
                .addFunction( "length", &ce::math::Vector4f::length )
                .addFunction( "squaredLength", &ce::math::Vector4f::squaredLength )
            .endClass()
        .endNamespace();
        
}