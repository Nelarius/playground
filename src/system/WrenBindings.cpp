
#include "Wren++.h"
#include "math/Vector.h"
#include "math/Quaternion.h"
#include "math/Geometry.h"
#include "ecs/Include.h"
#include "component/Include.h"
#include "imgui/imgui.h"
#include "utils/RingBuffer.h"
#include "utils/StringId.h"
#include "utils/Random.h"
#include "system/WrenBindingsImpl.h"
#include <cmath>
#include <vector>
#include <cstdlib>

namespace pg {
namespace wren {

extern void arrayPushBack(WrenVM*);
extern void arrayAt(WrenVM*);
extern void set(WrenVM*);

namespace ig = ImGui;

void bindMathModule() {
    wrenpp::beginModule("pg/math")
        .beginClass("Math")
            .bindFunction< decltype(static_cast<double(*)(double)>(&cos)), &cos     >(true, "cos(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&sin)), &sin     >(true, "sin(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&tan)), &tan     >(true, "tan(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&acos)), &acos   >(true, "acos(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&asin)), &asin   >(true, "asin(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&atan)), &atan   >(true, "atan(_)")
            .bindFunction< decltype(static_cast<double(*)(double, double)>(&atan2)), &atan2 >(true, "atan2(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&exp)), &exp     >(true, "exp(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&log)), &log     >(true, "log(_)")
            .bindFunction< decltype(static_cast<double(*)(double, double)>(&pow)), &pow     >(true, "pow(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&sqrt)), &sqrt   >(true, "sqrt(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&cbrt)), &cbrt   >(true, "cbrt(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&ceil)), &ceil   >(true, "ceil(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&floor)), &floor >(true, "floor(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&round)), &round >(true, "round(_)")
            .bindFunction< decltype(static_cast<double(*)(double)>(&abs)), &abs    >(true, "abs(_)")
            .bindFunction< decltype(static_cast<double(*)(double, double)>(&std::fmin)), &std::fmin >(true, "min(_,_)")
            .bindFunction< decltype(static_cast<double(*)(double, double)>(&std::fmax)), &std::fmax>(true, "max(_,_)")
            .bindFunction< decltype(static_cast<double(*)(void)>(&pg::randd)), static_cast<double(*)(void)>(&pg::randd) >(true, "rand()")
            .bindCFunction(true, "generateCameraRay(_,_)", wren::generateCameraRay)
        .endClass()
        .bindClass<math::Rayf, math::Vec3f, math::Vec3f>("Ray")
            .bindGetter<decltype(math::Rayf::origin), &math::Rayf::origin>("origin")
            .bindSetter<decltype(math::Rayf::origin), &math::Rayf::origin>("origin=(_)")
            .bindGetter<decltype(math::Rayf::direction), &math::Rayf::direction>("direction")
            .bindSetter<decltype(math::Rayf::direction), &math::Rayf::direction>("direction=(_)")
            .bindGetter<decltype(math::Rayf::t), &math::Rayf::t>("scalingParameter")
            .bindSetter<decltype(math::Rayf::t), &math::Rayf::t>("scalingParameter=(_)")
            .bindCFunction(false, "planeIntersection(_)", wren::planeIntersection)
        .endClass()
        .bindClass<math::Planef, math::Vec3f, math::Vec3f, math::Vec3f>("Plane")
            .bindMethod<decltype(&math::Planef::point), &math::Planef::point>(false, "point()")
            .bindMethod<decltype(&math::Planef::normal), &math::Planef::normal>(false, "normal()")
        .endClass()
    .endModule();
}

void bindRandomModule() {
    wrenpp::beginModule("pg/random")
        .beginClass("Random")
            .bindFunction<decltype(&randomize), &randomize>(true, "randomize()")
            .bindFunction<decltype(&seed), &seed>(true, "seed(_)")
            .bindFunction<decltype(&randi), &randi>(true, "int(_,_)")
            .bindFunction<decltype(static_cast<double(*)(double, double)>(&randd)), static_cast<double(*)(double, double)>(&randd)>(true, "real(_,_)")
            .bindFunction<decltype(static_cast<double(*)()>(&randd)), static_cast<double(*)()>(&randd)>(true, "real()")
        .endClass();
}

void bindImguiModule() {
    wrenpp::beginModule("pg/imgui")
        .bindClass< wren::ImGuiWindowFlag >("ImguiFlag")
            .bindCFunction(false, "setTitleBar()", wren::setTitleBar)
            .bindCFunction(false, "unsetTitleBar()", wren::unsetTitleBar)
            .bindCFunction(false, "setResize()", wren::setResize)
            .bindCFunction(false, "unsetResize()", wren::unsetResize)
            .bindCFunction(false, "setMove()", wren::setMove)
            .bindCFunction(false, "unsetMove()", wren::unsetMove)
            .bindCFunction(false, "setShowBorders()", wren::setShowBorders)
            .bindCFunction(false, "unsetShowBorders()", wren::unsetShowBorders)
            .endClass()
            .beginClass("Imgui")
            // windows & their formatting
            .bindCFunction(true, "begin(_)", wren::begin)
            .bindCFunction(true, "begin(_,_)", wren::beginWithFlags)
            .bindFunction< decltype(&ig::End), &ig::End>(true, "end()")
            .bindCFunction(true, "setNextWindowPos(_)", wren::setWindowPos)
            .bindCFunction(true, "setNextWindowSize(_)", wren::setWindowSize)
            .bindFunction< decltype(static_cast<bool(*)(const char*)>(&ig::TreeNode)), &ig::TreeNode >(true, "treeNode(_)")
            .bindFunction< decltype(&ig::Spacing), &ig::Spacing >(true, "spacing()")
            .bindFunction< decltype(&ig::Indent), &ig::Indent >(true, "indent()")
            .bindFunction< decltype(&ig::Unindent), &ig::Unindent >(true, "unindent()")
            .bindFunction< decltype(&ig::TreePop), &ig::TreePop>(true, "treePop()")
            .bindCFunction(true, "text(_)", wren::text)
            .bindCFunction(true, "textColored(_,_)", wren::textColored)
            .bindCFunction(true, "dummy(_)", wren::dummy)
            // elements
            .bindCFunction(true, "bulletText(_)", wren::bulletText)
            .bindFunction< decltype(&ig::Bullet), &ig::Bullet >(true, "bullet()")
            .bindCFunction(true, "button(_,_)", wren::buttonSized)
            .bindCFunction(true, "button(_)", wren::button)
            .bindFunction< decltype(static_cast<bool(*)(const char*, bool)>(&ig::RadioButton)), static_cast<bool(*)(const char*, bool)>(&ig::RadioButton) >(true, "radioButton(_,_)")
            .bindCFunction(true, "checkBox(_,_)", wren::checkBox)
            .bindCFunction( true, "plotArray(_,_,_)", wren::plotNumberArray )
            .bindCFunction( true, "plotArray(_,_,_,_)", wren::plotNumberArrayWithOffset )
            .bindCFunction( true, "plotArray(_,_,_,_,_)", wren::plotNumberArrayWithOffsetAndSize )
            .bindCFunction( true, "plotRingBuffer(_,_)", wren::plotRingBuffer )
            .bindCFunction( true, "plotRingBuffer(_,_,_)", wren::plotRingBufferWithSize )
            .bindCFunction( true, "sliderFloat(_,_,_,_)", wren::sliderFloat)
        .endClass();
}

void bindNumberArrayModule() {
    wrenpp::beginModule( "pg/array" )
        .bindClass< std::vector<float> >( "NumberArray" )
            .bindCFunction( false, "pushBack(_)", wren::arrayPushBack )
            .bindCFunction( false, "at(_)", wren::arrayAt )
        .endClass();
}

void bindRingBufferModule() {
    wrenpp::beginModule( "pg/ringbuffer" )
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
    wrenpp::beginModule( "pg/vector" )
        .bindClass< math::Vec3f, float, float, float >( "Vec3" )
            // properties
            .bindGetter< decltype(math::Vec3f::x), &math::Vec3f::x >( "x" )
            .bindSetter< decltype(math::Vec3f::x), &math::Vec3f::x >( "x=(_)" )
            .bindGetter< decltype(math::Vec3f::y), &math::Vec3f::y >( "y" )
            .bindSetter< decltype(math::Vec3f::y), &math::Vec3f::y >( "y=(_)" )
            .bindGetter< decltype(math::Vec3f::z), &math::Vec3f::z >( "z" )
            .bindSetter< decltype(math::Vec3f::z), &math::Vec3f::z >( "z=(_)" )
            .bindGetter< decltype(math::Vec3f::x), &math::Vec3f::x >( "r" )
            .bindSetter< decltype(math::Vec3f::x), &math::Vec3f::x >( "r=(_)" )
            .bindGetter< decltype(math::Vec3f::y), &math::Vec3f::y >( "g" )
            .bindSetter< decltype(math::Vec3f::y), &math::Vec3f::y >( "g=(_)" )
            .bindGetter< decltype(math::Vec3f::z), &math::Vec3f::z >( "b" )
            .bindSetter< decltype(math::Vec3f::z), &math::Vec3f::z >( "b=(_)" )
            // methods
            .bindMethod< decltype(&math::Vec3f::norm), &math::Vec3f::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Vec3f::normalize), &math::Vec3f::normalize>(false, "normalize()")
            .bindMethod< decltype(&math::Vec3f::normSquared), &math::Vec3f::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Vec3f::dot), &math::Vec3f::dot >( false, "dot(_)" )
            .bindMethod< decltype(&math::Vec3f::operator+), &math::Vec3f::operator+ >(false, "+(_)")
            .bindMethod< decltype(&math::Vec3f::operator-), &math::Vec3f::operator- >(false, "-(_)")
            .bindMethod< decltype(&math::Vec3f::cross), &math::Vec3f::cross>(false, "cross(_)")
            .bindMethod< decltype(&math::Vec3f::hadamard), &math::Vec3f::hadamard>(false, "*(_)")
            .bindMethod< decltype(&math::Vec3f::operator*), &math::Vec3f::operator*>(false, "scale(_)")
        .endClass()
        .bindClass< math::Vec2f, float, float >( "Vec2" )
            .bindGetter< decltype(math::Vec2f::x), &math::Vec2f::x >( "x" )
            .bindSetter< decltype(math::Vec2f::x), &math::Vec2f::x >( "x=(_)" )
            .bindGetter< decltype(math::Vec2f::y), &math::Vec2f::y >( "y" )
            .bindSetter< decltype(math::Vec2f::y), &math::Vec2f::y >( "y=(_)" )
            .bindGetter< decltype(math::Vec2f::x), &math::Vec2f::x >( "i")
            .bindSetter< decltype(math::Vec2f::x), &math::Vec2f::x >( "i=(_)")
            .bindGetter< decltype(math::Vec2f::y), &math::Vec2f::y >( "j")
            .bindSetter< decltype(math::Vec2f::y), &math::Vec2f::y >( "j=(_)")
            .bindMethod< decltype(&math::Vec2f::norm), &math::Vec2f::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Vec2f::normalize), &math::Vec2f::normalize>(false, "normalize()")
            .bindMethod< decltype(&math::Vec2f::normSquared), &math::Vec2f::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Vec2f::dot), &math::Vec2f::dot >( false, "dot(_)" )
            .bindMethod< decltype(&math::Vec2f::hadamard), &math::Vec2f::hadamard>(false, "*(_)")
            .bindMethod< decltype(&math::Vec2f::operator+), &math::Vec2f::operator+ >(false, "+(_)")
            .bindMethod< decltype(&math::Vec2f::operator-), &math::Vec2f::operator- >(false, "-(_)")
            .bindMethod< decltype(&math::Vec2f::operator*), &math::Vec2f::operator*>(false, "scale(_)")
        .endClass()
        .bindClass< math::Vec4f, float, float, float, float >( "Vec4" )
            .bindGetter< decltype(math::Vec4f::x), &math::Vec4f::x >( "x" )
            .bindSetter< decltype(math::Vec4f::x), &math::Vec4f::x >( "x=(_)" )
            .bindGetter< decltype(math::Vec4f::y), &math::Vec4f::y >( "y" )
            .bindSetter< decltype(math::Vec4f::y), &math::Vec4f::y >( "y=(_)" )
            .bindGetter< decltype(math::Vec4f::z), &math::Vec4f::z >( "z" )
            .bindSetter< decltype(math::Vec4f::z), &math::Vec4f::z >( "z=(_)" )
            .bindGetter< decltype(math::Vec4f::w), &math::Vec4f::w >( "w" )
            .bindSetter< decltype(math::Vec4f::w), &math::Vec4f::w >( "w=(_)" )
            .bindGetter< decltype(math::Vec4f::x), &math::Vec4f::x >( "r")
            .bindSetter< decltype(math::Vec4f::x), &math::Vec4f::x >( "r=(_)")
            .bindGetter< decltype(math::Vec4f::y), &math::Vec4f::y >( "g")
            .bindSetter< decltype(math::Vec4f::y), &math::Vec4f::y >( "g=(_)")
            .bindGetter< decltype(math::Vec4f::z), &math::Vec4f::z >( "b")
            .bindSetter< decltype(math::Vec4f::z), &math::Vec4f::z >( "b=(_)")
            .bindGetter< decltype(math::Vec4f::w), &math::Vec4f::w >( "a")
            .bindSetter< decltype(math::Vec4f::w), &math::Vec4f::w >( "a=(_)")
            .bindMethod< decltype(&math::Vec4f::norm), &math::Vec4f::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Vec4f::normalize), &math::Vec4f::normalize>(false, "normalize()")
            .bindMethod< decltype(&math::Vec4f::normSquared), &math::Vec4f::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Vec4f::dot), &math::Vec4f::dot >( false, "dot(_)" )
            .bindMethod< decltype(&math::Vec4f::hadamard), &math::Vec4f::hadamard>(false, "*(_)")
            .bindMethod< decltype(&math::Vec4f::operator+), &math::Vec4f::operator+ >(false, "+(_)")
            .bindMethod< decltype(&math::Vec4f::operator-), &math::Vec4f::operator- >(false, "-(_)")
            .bindMethod< decltype(&math::Vec4f::operator*), &math::Vec4f::operator*>(false, "scale(_)")
        .endClass()
    .endModule();
}

void bindQuaternionModule() {
    wrenpp::beginModule( "pg/quaternion" )
        .bindClass< math::Quatf, float, float, float, float >( "Quat" )
            // properties
            .bindSetter< decltype(math::Quatf::v), &math::Quatf::v >( "v=(_)")
            .bindCFunction(false, "v", wren::getQuatImaginary)
            .bindGetter< decltype(math::Quatf::w), &math::Quatf::w >( "w" )
            .bindSetter< decltype(math::Quatf::w), &math::Quatf::w >( "w=(_)" )
            // methods
            .bindMethod< decltype(&math::Quatf::norm), &math::Quatf::norm >( false, "norm()" )
            .bindMethod< decltype(&math::Quatf::normSquared), &math::Quatf::normSquared >( false, "normSquared()" )
            .bindMethod< decltype(&math::Quatf::normalize), &math::Quatf::normalize>(false, "normalize()")
            .bindMethod< decltype(&math::Quatf::angle), &math::Quatf::angle >( false, "angle()" )
            .bindMethod<decltype(&math::Quatf::conjugate), &math::Quatf::conjugate>(false, "conjugate()")
            .bindMethod<decltype(&math::Quatf::inverse), &math::Quatf::inverse>(false, "inverse()")
            .bindMethod<decltype(&math::Quatf::axis), &math::Quatf::axis>(false, "axis()")
            .bindMethod<decltype(&math::Quatf::multiply), &math::Quatf::multiply>(false, "*(_)")
            .bindMethod<decltype(&math::Quatf::xaxis), &math::Quatf::xaxis>(false, "xaxis")
            .bindMethod<decltype(&math::Quatf::yaxis), &math::Quatf::yaxis>(false, "yaxis")
            .bindMethod<decltype(&math::Quatf::zaxis), &math::Quatf::zaxis>(false, "zaxis")
        .endClass()
    .endModule();
}

void bindEntityModule() {
    wrenpp::beginModule("pg/entity")
        .bindClass< ecs::Entity >("Entity")
            .bindMethod<decltype(&ecs::Entity::isValid), &ecs::Entity::isValid>(false, "isValid()")
            .bindMethod<decltype(&ecs::Entity::destroy), &ecs::Entity::destroy>(false, "destroy()")
            .bindMethod<decltype(&ecs::Entity::invalidate), &ecs::Entity::invalidate>(false, "invalidate()")
            .bindCFunction(false, "hasTransform()", wren::hasTransform)
            .bindCFunction(false, "hasRenderable()", wren::hasRenderable)
            .bindCFunction(false, "hasCamera()", wren::hasCamera)
            .bindCFunction(false, "hasPointLight()", wren::hasPointLight)
            .bindCFunction(false, "assignTransform(_)", wren::assignTransform)
            .bindCFunction(false, "assignRenderable(_)", wren::assignRenderable)
            .bindCFunction(false, "set_(_)", wren::set)
            .bindCFunction(false, "index", wren::entityIndex)
            .bindCFunction(false, "version", wren::entityVersion)
            .bindCFunction(false, "transform", wren::getTransform)
            .bindCFunction(false, "transform=(_)", wren::setTransform)
            .bindCFunction(false, "renderable", wren::getRenderable)
            .bindCFunction(false, "renderable=(_)", wren::setRenderable)
        .endClass()
        .beginClass("EntityManager")
            .bindCFunction(true, "create()", wren::createEntity)
            .bindCFunction(true, "entityCount", wren::entityCount)
        .endClass()
    .endModule();
    wrenpp::beginModule("pg/event")
        .beginClass("EventManager")
            .bindCFunction(true, "listenToKeyDown(_,_)", wren::listenToKeyDown)
            .bindCFunction(true, "listenToKeyPressed(_,_)", wren::listenToKeyPressed)
            .bindCFunction(true, "listenToKeyUp(_,_)", wren::listenToKeyUp)
            .bindCFunction(true, "listenToMouseDown(_,_)", wren::listenToMouseDown)
            .bindCFunction(true, "listenToMousePressed(_,_)", wren::listenToMousePressed)
            .bindCFunction(true, "listenToMouseUp(_,_)", wren::listenToMouseUp)
            .bindCFunction(true, "listenToMouseScroll(_)", wren::listenToMouseScroll)
        .endClass()
    .endModule();
    wrenpp::beginModule("pg/mouse")
        .beginClass("Mouse")
            .bindCFunction(true, "x", wren::mouseX)
            .bindCFunction(true, "y", wren::mouseY)
            .bindCFunction(true, "dx", wren::mouseDx)
            .bindCFunction(true, "dy", wren::mouseDy)
        .endClass()
    .endModule();
}

void bindComponentModule() {
    wrenpp::beginModule("pg/component")
        .bindClass<component::Transform, math::Vec3f, math::Quatf, math::Vec3f>("Transform")
            .bindCFunction(false, "position", wren::getTransformPosition)
            .bindCFunction(false, "rotation", wren::getTransformRotation)
            .bindCFunction(false, "scale", wren::getTransformScale)
            .bindSetter<decltype(component::Transform::position), &component::Transform::position>("position=(_)")
            .bindSetter<decltype(component::Transform::rotation), &component::Transform::rotation>("rotation=(_)")
            .bindSetter<decltype(component::Transform::scale), &component::Transform::scale>("scale=(_)")
        .endClass()
        .bindClass<WrenRenderable, StringId, StringId>("Renderable")
            .bindSetter<decltype(WrenRenderable::shininess), &WrenRenderable::shininess>("shininess=(_)")
            .bindGetter<decltype(WrenRenderable::shininess), &WrenRenderable::shininess>("shininess")
            .bindSetter<decltype(WrenRenderable::ambientColor), &WrenRenderable::ambientColor>("ambientColor=(_)")
            .bindGetter<decltype(WrenRenderable::ambientColor), &WrenRenderable::ambientColor>("ambientColor")
            .bindSetter<decltype(WrenRenderable::baseColor), &WrenRenderable::baseColor>("baseColor=(_)")
            .bindGetter<decltype(WrenRenderable::baseColor), &WrenRenderable::baseColor>("baseColor")
            .bindSetter<decltype(WrenRenderable::specularColor), &WrenRenderable::specularColor>("specularColor=(_)")
            .bindGetter<decltype(WrenRenderable::specularColor), &WrenRenderable::specularColor>("specularColor")
        .endClass()
   .endModule();
}

void bindUtilsModule() {
    wrenpp::beginModule("pg/utils")
        .bindClass<StringId, const char*>("ResourceHandle")
            .bindMethod<decltype(&StringId::cString), &StringId::cString>(false, "toString")
        .endClass()
    .endModule();
}

void bindSystemsModule() {
    wrenpp::beginModule("pg/systems")
        .beginClass("Pick3d")
            .bindCFunction(true, "castCameraRay(_,_)", wren::castCameraRay)
        .endClass()
        .beginClass("DebugRenderer")
            .bindCFunction(true, "addDebugBox(_,_,_,_)", wren::addTransientDebugBox)
            .bindCFunction(true, "addDebugBox(_,_,_)", wren::addStaticDebugBox)
            .bindCFunction(true, "addDebugLine(_,_,_,_)", wren::addTransientDebugLine)
            .bindCFunction(true, "addDebugLine(_,_,_)", wren::addStaticDebugLine)
        .endClass()
    .endModule();
}

}   // wren
}   // pg
