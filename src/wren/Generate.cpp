#include "wren/Include.h"
#include "utils/Random.h"
#include "math/Vector.h"
#include "wren/WrenVector.h"
#include "math/Quaternion.h"
#include "wren/WrenQuaternion.h"
#include "ecs/Include.h"
#include "component/Include.h"
#include "wren/WrenEntity.h"
#include "imgui/imgui.h"
#include "wren/WrenImgui.h"
#include "wren/WrenArray.h"
#include "wren/WrenRingBuffer.h"
#include "utils/RingBuffer.h"
#include <cmath>
#include <vector>
#include <cstdlib>

namespace pg {
namespace wren {

namespace ig = ImGui;

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

void bindImguiModule() {
    wrenly::beginModule( "builtin/imgui" )
        .bindClass< wren::ImGuiWindowFlag >( "ImguiFlag" )
            .bindCFunction( false, "setTitleBar()", wren::setTitleBar )
            .bindCFunction( false, "unsetTitleBar()", wren::unsetTitleBar )
            .bindCFunction( false, "setResize()", wren::setResize )
            .bindCFunction( false, "unsetResize()", wren::unsetResize )
            .bindCFunction( false, "setMove()", wren::setMove )
            .bindCFunction( false, "unsetMove()", wren::unsetMove )
            .bindCFunction( false, "setShowBorders()", wren::setShowBorders )
            .bindCFunction( false, "unsetShowBorders()", wren::unsetShowBorders )
        .endClass()
        .beginClass( "Imgui" )
            // windows & their formatting
            .bindCFunction( true, "begin(_)", wren::begin )
            .bindCFunction( true, "begin(_,_)", wren::beginWithFlags )
            .bindFunction< decltype(&ig::End), &ig::End>( true, "end()" )
            .bindCFunction( true, "setNextWindowPos(_)", wren::setWindowPos )
            .bindCFunction( true, "setNextWindowSize(_)", wren::setWindowSize )
            .bindFunction< decltype(static_cast<bool(*)(const char*)>(&ig::TreeNode)), &ig::TreeNode >( true, "treeNode(_)" )
            .bindFunction< decltype(&ig::Spacing), &ig::Spacing >( true, "spacing()" )
            .bindFunction< decltype(&ig::Indent), &ig::Indent >( true, "indent()" )
            .bindFunction< decltype(&ig::Unindent), &ig::Unindent >( true, "unindent()" )
            .bindFunction< decltype(&ig::TreePop), &ig::TreePop>( true, "treePop()" )
            .bindCFunction( true, "text(_)", wren::text )
            .bindCFunction( true, "textColored(_,_)", wren::textColored )
            .bindCFunction( true, "dummy(_)", wren::dummy )
            // elements
            .bindCFunction( true, "bulletText(_)", wren::bulletText )
            .bindFunction< decltype(&ig::Bullet), &ig::Bullet >(true, "bullet()" )
            .bindCFunction( true, "button(_,_)", wren::buttonSized )
            .bindCFunction( true, "button(_)", wren::button )
            .bindFunction< decltype(static_cast<bool(*)(const char*, bool)>(&ig::RadioButton)), static_cast<bool(*)(const char*, bool)>(&ig::RadioButton) >( true, "radioButton(_,_)" )
            .bindCFunction( true, "plotArray(_,_,_)", wren::plotNumberArray )
            .bindCFunction( true, "plotArray(_,_,_,_)", wren::plotNumberArrayWithOffset )
            .bindCFunction( true, "plotArray(_,_,_,_,_)", wren::plotNumberArrayWithOffsetAndSize )
            .bindCFunction( true, "plotRingBuffer(_,_)", wren::plotRingBuffer )
            .bindCFunction( true, "plotRingBuffer(_,_,_)", wren::plotRingBufferWithSize )
        .endClass();
}

void bindNumberArrayModule() {
    wrenly::beginModule( "builtin/array" )
        .bindClass< std::vector<float> >( "NumberArray" )
            .bindCFunction( false, "pushBack(_)", wren::arrayPushBack )
            .bindCFunction( false, "at(_)", wren::arrayAt )
        .endClass();
}

void bindRingBufferModule() {
    wrenly::beginModule( "builtin/ringbuffer" )
        .bindClass< RingBuffer<float>, std::size_t >( "NumberRingBuffer" )
            // element access
            .bindMethod< decltype((float&(RingBuffer<float>::*)(std::size_t))&RingBuffer<float>::at), &RingBuffer<float>::at >( false, "at(_)" )
            .bindMethod< decltype((float&(RingBuffer<float>::*)())&RingBuffer<float>::front), &RingBuffer<float>::front >( false, "front()" )
            .bindMethod< decltype((float&(RingBuffer<float>::*)())&RingBuffer<float>::back), &RingBuffer<float>::back >( false, "back()" )
            // modifiers
            //.bindMethod< decltype(&RingBuffer<float>::pushBack), &RingBuffer<float>::pushBack >( false, "pushBack(_)" )
            .bindCFunction( false, "pushBack(_)", wren::ringBufferPushBack )
            .bindMethod< decltype(&RingBuffer<float>::popBack), &RingBuffer<float>::popBack >( false, "popBack()" )
            .bindMethod< decltype(&RingBuffer<float>::popFront), &RingBuffer<float>::popFront >( false, "popFront()" )
            .bindMethod< decltype(&RingBuffer<float>::clear), &RingBuffer<float>::clear >( false, "clear()" )
            // container data
            .bindMethod< decltype(&RingBuffer<float>::size), &RingBuffer<float>::size >( false, "size" )
            .bindMethod< decltype(&RingBuffer<float>::capacity ), &RingBuffer<float>::capacity >( false, "capacity" )
        .endClass();
}

void bindVectorModule() {
    wrenly::beginModule( "builtin/vector" )
        .bindClass< math::Vector3f, float, float, float >( "Vec3" )
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
        .bindClass< math::Vector2f, float, float >( "Vec2" )
            .bindGetter< decltype(math::Vector2f::x), &math::Vector2f::x >( false, "x" )
            .bindSetter< decltype(math::Vector2f::x), &math::Vector2f::x >( false, "x=(_)" )
            .bindGetter< decltype(math::Vector2f::y), &math::Vector2f::y >( false, "y" )
            .bindSetter< decltype(math::Vector2f::y), &math::Vector2f::y >( false, "y=(_)" )
            .bindMethod< decltype(&math::Vector2f::norm), &math::Vector2f::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Vector2f::normSquared), math::Vector2f::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Vector2f::dot), math::Vector2f::dot >( false, "dot(_)" )
        .endClass()
        .bindClass< math::Vector4f, float, float, float, float >( "Vec4" )
            .bindGetter< decltype(math::Vector4f::x), &math::Vector4f::x >( false, "x" )
            .bindSetter< decltype(math::Vector4f::x), &math::Vector4f::x >( false, "x=(_)" )
            .bindGetter< decltype(math::Vector4f::y), &math::Vector4f::y >( false, "y" )
            .bindSetter< decltype(math::Vector4f::y), &math::Vector4f::y >( false, "y=(_)" )
            .bindGetter< decltype(math::Vector4f::z), &math::Vector4f::z >( false, "z" )
            .bindSetter< decltype(math::Vector4f::z), &math::Vector4f::z >( false, "z=(_)" )
            .bindGetter< decltype(math::Vector4f::w), &math::Vector4f::w >( false, "w" )
            .bindSetter< decltype(math::Vector4f::w), &math::Vector4f::w >( false, "w=(_)" )
            .bindMethod< decltype(&math::Vector4f::norm), &math::Vector4f::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Vector4f::normSquared), math::Vector4f::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Vector4f::dot), math::Vector4f::dot >( false, "dot(_)" )
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
