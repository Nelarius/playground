#include "wren/Include.h"
#include "utils/Random.h"
#include "math/Vector.h"
#include "wren/WrenVector.h"
#include "math/Quaternion.h"
#include "wren/WrenQuaternion.h"
#include "ecs/Include.h"
#include "component/Include.h"
#include "wren/WrenEntity.h"
#include <cmath>

namespace pg {
namespace wren {

void bindMathModule() {
    wrenly::beginModule( "builtin/math" )
        .beginClass( "Math" )
            .bindFunction< decltype(&cos), &cos     >( true, "cos(_)" )
            .bindFunction< decltype(&sin), &sin     >( true, "sin(_)" )
            .bindFunction< decltype(&tan), &tan     >( true, "tan(_)" )
            .bindFunction< decltype(&acos), &acos   >( true, "acos(_)" )
            .bindFunction< decltype(&asin), &asin   >( true, "asin(_)" )
            .bindFunction< decltype(&atan), &atan   >( true, "atan(_)" )
            .bindFunction< decltype(&atan2), &atan2 >( true, "atan2(_)" )
            .bindFunction< decltype(&exp), &exp     >( true, "exp(_)" )
            .bindFunction< decltype(&log), &log     >( true, "log(_)" )
            .bindFunction< decltype(&pow), &pow     >( true, "pow(_)" )
            .bindFunction< decltype(&sqrt), &sqrt   >( true, "sqrt(_)" )
            .bindFunction< decltype(&cbrt), &cbrt   >( true, "cbrt(_)" )
            .bindFunction< decltype(&ceil), &ceil   >( true, "ceil(_)" )
            .bindFunction< decltype(&floor), &floor >( true, "floor(_)" )
            .bindFunction< decltype(&round), &round >( true, "round(_)" )
            .bindFunction< decltype(&abs),  &abs    >( true, "abs(_)" )
            .bindFunction< decltype(static_cast<double(*)(double, double)>(&pg::Randd) ), static_cast<double(*)(double, double)>(&pg::Randd) >( true, "rand(_,_)" )
            .bindFunction< decltype(static_cast<double(*)(void)>(&pg::Randd)), static_cast<double(*)(void)>(&pg::Randd) >( true, "rand()" );
}

void bindVectorModule() {
    wrenly::beginModule( "builtin/vector" )
        .bindClass< math::Vector3f, float, float, float >( "Vec3" )
            // properties
            // properties 
            .bindGetter< decltype(math::Vector3f::x), &math::Vector3f::x >( false, "x" )
            .bindSetter< decltype(math::Vector3f::x), &math::Vector3f::x >( false, "x=(_)" )
            .bindGetter< decltype(math::Vector3f::y), &math::Vector3f::y >( false, "y" )
            .bindSetter< decltype(math::Vector3f::y), &math::Vector3f::y >( false, "y=(_)" )
            .bindGetter< decltype(math::Vector3f::z), &math::Vector3f::z >( false, "z" )
            .bindSetter< decltype(math::Vector3f::z), &math::Vector3f::z >( false, "z=(_)" )
            // methods
            .bindMethod< decltype(&math::Vector3f::norm), &math::Vector3f::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Vector3f::normSquared), &math::Vector3f::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Vector3f::dot), &math::Vector3f::dot >( false, "dot(_)" )
            .bindCFunction( false, "cross(_)", wren::cross3f )
            .bindCFunction( false, "plus(_)", wren::plus3f )
            .bindCFunction( false, "minus(_)", wren::minus3f )
            .bindCFunction( false, "hadamard(_)", wren::hadamard3f )
        .endClass()
    .endModule();
}

void bindQuaternionModule() {
    wrenly::beginModule( "builtin/quaternion" )
        .bindClass< math::Quatf, float, float, float, float >( "Quat" )
            // properties
            .bindSetter< decltype(math::Quatf::v), &math::Quatf::v >( false, "v=(_)")
            .bindGetter< decltype(math::Quatf::w), &math::Quatf::w >( false, "w" )
            .bindSetter< decltype(math::Quatf::w), &math::Quatf::w >( false, "w=(_)" )
            // methods
            .bindMethod< decltype(&math::Quatf::norm), &math::Quatf::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Quatf::normSquared), &math::Quatf::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Quatf::angle), &math::Quatf::angle >( false, "angle()" )
            .bindCFunction( false, "conjugate()", wren::conjugate )
            .bindCFunction( false, "inverse()", wren::inverse )
            .bindCFunction( false, "axis()", wren::axis )
            .bindCFunction( false, "multiply(_)", wren::multiply )
        .endClass()
    .endModule();
}

void bindEntityModule() {
    wrenly::beginModule( "builtin/entity" )
        .bindClass< ecs::Entity >( "Entity" )
            .bindMethod< decltype(&ecs::Entity::isValid), &ecs::Entity::isValid >( false, "isValid()" )
            .bindMethod< decltype(&ecs::Entity::has<component::Transform>), &ecs::Entity::has<component::Transform> >( false, "hasTransform()" )
            .bindMethod< decltype(&ecs::Entity::has<component::Renderable>), &ecs::Entity::has<component::Renderable> >( false, "hasRenderable()" )
            .bindMethod< decltype(&ecs::Entity::has<component::Camera>), &ecs::Entity::has<component::Camera> >( false, "hasCamera()" )
            .bindMethod< decltype(&ecs::Entity::has<component::PointLight>), &ecs::Entity::has<component::PointLight> >( false, "hasPointLight()" )
            .bindCFunction( false, "set(_)", wren::set )
            .bindCFunction( false, "index", wren::entityIndex )
            .bindCFunction( false, "version", wren::entityVersion )
            .bindCFunction( false, "transform=(_)", wren::setTransform )
        .endClass()
        .bindClass< component::Transform, math::Vector3f, math::Quatf, math::Vector3f >( "Transform" )
        .endClass()
    .endModule();
}

}   // wren
}   // pg
