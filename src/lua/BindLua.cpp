#include "lua/BindLua.h"
#include "math/Vector.h"
#include <LuaBridge/LuaBridge.h>

void pg::BindAll( lua_State* l ) {
    pg::BindVector( l );
}

void pg::BindVector( lua_State* l ) {
    luabridge::getGlobalNamespace( l )
        .beginNamespace( "pg" )
            .beginClass<pg::math::Vector2f>( "Vector2f" )
                .addConstructor<void(*)( float, float )>()
                .addData( "x", &pg::math::Vector2f::x )
                .addData( "y", &pg::math::Vector2f::y )
                .addData( "r", &pg::math::Vector2f::r )
                .addData( "g", &pg::math::Vector2f::g )
                .addFunction( "l2norm", &pg::math::Vector2f::l2norm )
                .addFunction( "normalize", &pg::math::Vector2f::normalize )
                .addFunction( "length", &pg::math::Vector2f::length )
                .addFunction( "squaredLength", &pg::math::Vector2f::squaredLength )
            .endClass()
            .beginClass<pg::math::Vector3f>( "Vector3f" )
                .addConstructor<void(*)( float, float, float )>()
                .addData( "x", &pg::math::Vector3f::x )
                .addData( "y", &pg::math::Vector3f::y )
                .addData( "z", &pg::math::Vector3f::z )
                .addData( "r", &pg::math::Vector3f::r )
                .addData( "g", &pg::math::Vector3f::g )
                .addData( "b", &pg::math::Vector3f::b )
                .addFunction( "l2norm", &pg::math::Vector3f::l2norm )
                .addFunction( "normalize", &pg::math::Vector3f::normalize )
                .addFunction( "length", &pg::math::Vector3f::length )
                .addFunction( "squaredLength", &pg::math::Vector3f::squaredLength )
                .addFunction( "cross", &pg::math::Vector3f::cross )
            .endClass()
            .beginClass<pg::math::Vector4f>( "Vector4f" )
                .addConstructor<void(*)( float, float, float, float )>()
                .addData( "x", &pg::math::Vector4f::x )
                .addData( "y", &pg::math::Vector4f::y )
                .addData( "z", &pg::math::Vector4f::z )
                .addData( "w", &pg::math::Vector4f::w )
                .addData( "r", &pg::math::Vector4f::r )
                .addData( "g", &pg::math::Vector4f::g )
                .addData( "b", &pg::math::Vector4f::b )
                .addData( "a", &pg::math::Vector4f::a )
                .addFunction( "l2norm", &pg::math::Vector4f::l2norm )
                .addFunction( "normalize", &pg::math::Vector4f::normalize )
                .addFunction( "length", &pg::math::Vector4f::length )
                .addFunction( "squaredLength", &pg::math::Vector4f::squaredLength )
            .endClass()
        .endNamespace();
        
}