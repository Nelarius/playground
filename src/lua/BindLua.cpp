#include "lua/BindLua.h"
#include "math/Vector.h"
#include "component/Include.h"
#include "ecs/Include.h"

namespace pg {

void BindAll( lua_State* l ) {
    BindVector( l );
    BindComponent( l );
    BindEntity( l );
}

void BindVector( lua_State* l ) {
    luabridge::getGlobalNamespace( l )
        .beginNamespace( "pg" )
            .beginClass<math::Vector2f>( "Vector2f" )
                .addConstructor<void(*)( float, float )>()
                .addData( "x", &math::Vector2f::x )
                .addData( "y", &math::Vector2f::y )
                .addData( "r", &math::Vector2f::r )
                .addData( "g", &math::Vector2f::g )
                .addFunction( "l2norm", &pg::math::Vector2f::l2norm )
                .addFunction( "normalize", &pg::math::Vector2f::normalize )
                .addFunction( "length", &pg::math::Vector2f::length )
                .addFunction( "squaredLength", &pg::math::Vector2f::squaredLength )
            .endClass()
            .beginClass<math::Vector3f>( "Vector3f" )
                .addConstructor<void(*)( float, float, float )>()
                .addData( "x", &math::Vector3f::x )
                .addData( "y", &math::Vector3f::y )
                .addData( "z", &math::Vector3f::z )
                .addData( "r", &math::Vector3f::r )
                .addData( "g", &math::Vector3f::g )
                .addData( "b", &math::Vector3f::b )
                .addFunction( "l2norm", &math::Vector3f::l2norm )
                .addFunction( "normalize", &math::Vector3f::normalize )
                .addFunction( "length", &math::Vector3f::length )
                .addFunction( "squaredLength", &math::Vector3f::squaredLength )
                .addFunction( "cross", &math::Vector3f::cross )
            .endClass()
            .beginClass<math::Vector4f>( "Vector4f" )
                .addConstructor<void(*)( float, float, float, float )>()
                .addData( "x", &math::Vector4f::x )
                .addData( "y", &math::Vector4f::y )
                .addData( "z", &math::Vector4f::z )
                .addData( "w", &math::Vector4f::w )
                .addData( "r", &math::Vector4f::r )
                .addData( "g", &math::Vector4f::g )
                .addData( "b", &math::Vector4f::b )
                .addData( "a", &math::Vector4f::a )
                .addFunction( "l2norm", &math::Vector4f::l2norm )
                .addFunction( "normalize", &math::Vector4f::normalize )
                .addFunction( "length", &math::Vector4f::length )
                .addFunction( "squaredLength", &math::Vector4f::squaredLength )
            .endClass()
        .endNamespace();
}

void BindComponent( lua_State* l ) {
    lb::getGlobalNamespace( l )
        .beginNamespace( "pg" )
            .beginClass<component::Transform>( "Transform" )
                .addData( "position", &component::Transform::position )
                .addData( "scale", &component::Transform::scale )
            .endClass()
            .beginClass<component::Camera>( "Camera" )
                .addData( "fov", &component::Camera::verticalFov )
                .addData( "nearPlane", &component::Camera::nearPlane )
                .addData( "farPlane", &component::Camera::farPlane )
                .addData( "viewPerspective", &component::Camera::viewPerspective )
                .addData( "active", &component::Camera::active )
            .endClass()
        .endNamespace();
}

void BindEntity( lua_State* l ) {
    lb::getGlobalNamespace( l )
        .beginNamespace( "pg" )
            .beginClass<ecs::Entity>( "Entity" )
                .addFunction( "isValid", &ecs::Entity::isValid )
                .addFunction( "hasTransform", &ecs::Entity::has<component::Transform> )
                .addFunction( "hasRenderable", &ecs::Entity::has<component::Renderable> )
                .addFunction( "hasCamera", &ecs::Entity::has<component::Camera> )
                .addFunction( "hasScript", &ecs::Entity::has<component::Script> )
                .addProperty( "transform", &ecs::Entity::componentPointer<component::Transform> )
                .addProperty( "camera", &ecs::Entity::componentPointer<component::Camera> )
                .addProperty( "renderable", &ecs::Entity::componentPointer<component::Renderable> )
            .endClass()
        .endNamespace();
}

}
