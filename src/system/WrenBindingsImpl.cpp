#include "imgui/imgui.h"
#include "app/MouseEvents.h"
#include "component/Include.h"
#include "math/Vector.h"
#include "manager/MeshManager.h"
#include "manager/ShaderManager.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "system/ScriptSystem.h"
#include "system/PickingSystem.h"
#include "system/WrenBindingsImpl.h"
#include "ecs/Include.h"
#include "utils/Locator.h"
#include "utils/Assert.h"
#include "utils/Log.h"
#include "utils/RingBuffer.h"
extern "C" {
#include <wren.h>
}
#include <vector>
#include <cstdlib>

namespace {

inline void returnQuatValue(WrenVM* vm, const pg::math::Quatf& quat) {
    wrenGetVariable(vm, "builtin/quaternion", "Quat", 0);
    void* data = wrenSetSlotNewForeign(vm, 0, 0, sizeof(pg::math::Quatf));
    memcpy(data, (void*)&quat, sizeof(pg::math::Quatf));
}

inline void returnVec2fValue(WrenVM* vm, const pg::math::Vec2f& result) {
    wrenGetVariable(vm, "builtin/vector", "Vec2", 0);
    void* data = wrenSetSlotNewForeign(vm, 0, 0, sizeof(pg::math::Vec2f));
    memcpy(data, (void*)&result, sizeof(pg::math::Vec2f));
}

inline void returnVec3fValue(WrenVM* vm, const pg::math::Vec3f& result) {
    wrenGetVariable(vm, "builtin/vector", "Vec3", 0);
    void* data = wrenSetSlotNewForeign(vm, 0, 0, sizeof(pg::math::Vec3f));
    memcpy(data, (void*)&result, sizeof(pg::math::Vec3f));
}

inline void returnVec4fValue(WrenVM* vm, const pg::math::Vec4f& result) {
    wrenGetVariable(vm, "builtin/vector", "Vec4", 0);
    void* data = wrenSetSlotNewForeign(vm, 0, 0, sizeof(pg::math::Vec4f));
    memcpy(data, (void*)&result, sizeof(pg::math::Vec4f));
}

inline void returnEntityValue(WrenVM* vm, pg::ecs::Entity& result) {
    wrenGetVariable(vm, "builtin/entity", "Entity", 0);
    void* data = wrenSetSlotNewForeign(vm, 0, 0, sizeof(pg::ecs::Entity));
    memcpy(data, (void*)&result, sizeof(pg::ecs::Entity));
}

}

namespace pg {
namespace wren {

/***
 *       _  __           __           ___
 *      / |/ /_ ____ _  / /  ___ ____/ _ | ___________ ___ __
 *     /    / // /  ' \/ _ \/ -_) __/ __ |/ __/ __/ _ `/ // /
 *    /_/|_/\_,_/_/_/_/_.__/\__/_/ /_/ |_/_/ /_/  \_,_/\_, /
 *                                                    /___/
 */

void arrayPushBack(WrenVM* vm) {
    std::vector<float>* array = (std::vector<float>*)wrenGetSlotForeign(vm, 0);
    array->push_back(float(wrenGetSlotDouble(vm, 1)));
}

void arrayAt(WrenVM* vm) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign(vm, 0);
    std::size_t i = (std::size_t)wrenGetSlotDouble(vm, 1);
    if (i >= array->size()) {
        LOG_ERROR << "Script error> NumberArray: index out of bounds. Size: " << array->size() << ", index: " << i;
        return;
    }
    wrenSetSlotDouble(vm, 0, double(array->at(i)));
}

/***
*       ____     __  _ __
*      / __/__  / /_(_) /___ __
*     / _// _ \/ __/ / __/ // /
*    /___/_//_/\__/_/\__/\_, /
*                       /___/
*/

void set(WrenVM* vm) {
    ecs::Entity* e = (ecs::Entity*)wrenGetSlotForeign(vm, 0);
    uint32_t id = static_cast<uint32_t>(wrenGetSlotDouble(vm, 1));
    ecs::EntityManager* entityManager = Locator< ecs::EntityManager >::get();
    *e = entityManager->get(id);
}

void entityIndex(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, double(e->id().index()));
}

void entityVersion(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, double(e->id().version()));
}

void setTransform(WrenVM* vm) {
    ecs::Entity* e = (ecs::Entity*)wrenGetSlotForeign(vm, 0);
    component::Transform* t = (component::Transform*)wrenGetSlotForeign(vm, 1);
    *e->componentPointer< component::Transform >() = *t;
}

void hasTransform(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->has<component::Transform>());
}

void hasRenderable(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->has<component::Renderable>());
}

void hasCamera(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->has<component::Camera>());
}

void hasPointLight(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, e->has<component::PointLight>());
}

void assignTransform(WrenVM* vm) {
    ecs::Entity* e = (ecs::Entity*)wrenGetSlotForeign(vm, 0);
    const component::Transform* t = (const component::Transform*)wrenGetSlotForeign(vm, 1);
    e->assign<component::Transform>(t->position, t->rotation, t->scale);
}

void assignRenderable(WrenVM* vm) {
    ecs::Entity* e = (ecs::Entity*)wrenGetSlotForeign(vm, 0);
    const WrenRenderable* r = (const WrenRenderable*)wrenGetSlotForeign(vm, 1);

    pg::opengl::BufferObject* buffer = Locator<pg::MeshManager>::get()->get(r->model.cString());
    pg::opengl::Program* shader = Locator<pg::ShaderManager>::get()->get(r->shader.cString());

    std::unordered_map<std::string, float> uniforms{};
    uniforms.emplace("specColor_r", r->specularColor.r);
    uniforms.emplace("specColor_g", r->specularColor.g);
    uniforms.emplace("specColor_b", r->specularColor.b);
    uniforms.emplace("ambientColor_r", r->ambientColor.r);
    uniforms.emplace("ambientColor_g", r->ambientColor.g);
    uniforms.emplace("ambientColor_b", r->ambientColor.b);
    uniforms.emplace("baseColor_r", r->baseColor.r);
    uniforms.emplace("baseColor_g", r->baseColor.g);
    uniforms.emplace("baseColor_b", r->baseColor.b);
    system::Material mat;
    mat.type = system::MaterialType::Specular;
    mat.uniforms = uniforms;
    opengl::VertexArrayObjectFactory factory{ buffer, shader };
    factory.addStandardAttribute(opengl::VertexAttribute::Vertex);
    factory.addStandardAttribute(opengl::VertexAttribute::Normal);
    auto vao = factory.getVao();

    e->assign<component::Renderable>(buffer, shader, vao, mat);

    const auto& bb = Locator<MeshManager>::get()->getBoundingBox(r->model.cString());
    e->assign<math::AABox>(bb.min, bb.max);
}

/***
*       ____     __  _ __       __  ___
*      / __/__  / /_(_) /___ __/  |/  /__ ____  ___ ____ ____ ____
*     / _// _ \/ __/ / __/ // / /|_/ / _ `/ _ \/ _ `/ _ `/ -_) __/
*    /___/_//_/\__/_/\__/\_, /_/  /_/\_,_/_//_/\_,_/\_, /\__/_/
*                       /___/                      /___/
*/

void createEntity(WrenVM* vm) {
    ecs::Entity entity = Locator<ecs::EntityManager>::get()->create();
    wrenGetVariable(vm, "builtin/entity", "Entity", 0);
    void* data = wrenSetSlotNewForeign(vm, 0, 0, sizeof(ecs::Entity));
    memcpy(data, (void*)&entity, sizeof(ecs::Entity));
}

void entityCount(WrenVM* vm) {
    wrenSetSlotDouble(vm, 0, double(Locator<ecs::EntityManager>::get()->size()));
}

/***
*       ____              __  __  ___
*      / __/  _____ ___  / /_/  |/  /__ ____  ___ ____ ____ ____
*     / _/| |/ / -_) _ \/ __/ /|_/ / _ `/ _ \/ _ `/ _ `/ -_) __/
*    /___/|___/\__/_//_/\__/_/  /_/\_,_/_//_/\_,_/\_, /\__/_/
*                                                /___/
*/

void listenToKeyDown(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptSystem>::get()->listenToKeyDown(wrenGetSlotString(vm, 2), entity);
}

void listenToKeyPressed(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptSystem>::get()->listenToKeyPressed(wrenGetSlotString(vm, 2), entity);
}

void listenToKeyUp(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptSystem>::get()->listenToKeyUp(wrenGetSlotString(vm, 2), entity);
}

void listenToMouseDown(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptSystem>::get()->listenToMouseDown(wrenGetSlotString(vm, 2), entity);
}

void listenToMousePressed(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptSystem>::get()->listenToMousePressed(wrenGetSlotString(vm, 2), entity);
}

void listenToMouseUp(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptSystem>::get()->listenToMouseUp(wrenGetSlotString(vm, 2), entity);
}

void mouseX(WrenVM* vm) {
    math::Vec2i coords = Locator<MouseEvents>::get()->getMouseCoords();
    wrenSetSlotDouble(vm, 0, double(coords.x));
}

void mouseY(WrenVM* vm) {
    math::Vec2i coords = Locator<MouseEvents>::get()->getMouseCoords();
    wrenSetSlotDouble(vm, 0, double(coords.y));
}

void mouseDx(WrenVM* vm) {
    math::Vec2i coords = Locator<MouseEvents>::get()->getMouseDelta();
    wrenSetSlotDouble(vm, 0, double(coords.x));
}

void mouseDy(WrenVM* vm) {
    math::Vec2i coords = Locator<MouseEvents>::get()->getMouseDelta();
    wrenSetSlotDouble(vm, 0, double(coords.y));
}

/***
 *       ____                _
 *      /  _/_ _  ___ ___ __(_)
 *     _/ //  ' \/ _ `/ // / /
 *    /___/_/_/_/\_, /\_,_/_/
 *              /___/
 */

void begin(WrenVM* vm) {
    ImGui::Begin((const char*)wrenGetSlotString(vm, 1), NULL, 0);
}

void beginWithFlags(WrenVM* vm) {
    ImGui::Begin(
        (const char*)wrenGetSlotString(vm, 1),
        NULL,
        *(uint32_t*)wrenGetSlotForeign(vm, 2)
        );
}

void setWindowPos(WrenVM* vm) {
    ImGui::SetNextWindowPos(*(const ImVec2*)wrenGetSlotForeign(vm, 1));
}

void setWindowSize(WrenVM* vm) {
    ImGui::SetNextWindowSize(*(const ImVec2*)wrenGetSlotForeign(vm, 1));
}

void text(WrenVM* vm) {
    const char* str = wrenGetSlotString(vm, 1);
    ImGui::Text(str);
}

void dummy(WrenVM* vm) {
    const math::Vec2f* v = (const math::Vec2f*)wrenGetSlotForeign(vm, 1);
    ImGui::Dummy((const ImVec2&)(*v));
}

void textColored(WrenVM* vm) {
    // do nothing for now
    //const math::Vector2f* v = (const math::Vector2f*)wrenGetSlotForeign( vm, 1 );
    //const char* t = wrenGetSlotString( vm, 2 );
    //ImGui::TextColored();
}
void bulletText(WrenVM* vm) {
    ImGui::BulletText((const char*)wrenGetSlotString(vm, 1));
}

void button(WrenVM* vm) {
    ImGui::Button((const char*)wrenGetSlotString(vm, 1));
}

void buttonSized(WrenVM* vm) {
    ImGui::Button(
        (const char*)wrenGetSlotString(vm, 1),
        *(const ImVec2*)wrenGetSlotForeign(vm, 2)
        );
}

void plotNumberArray(WrenVM* vm) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign(vm, 2);
    ImGui::PlotLines(
        wrenGetSlotString(vm, 1),
        array->data(),
        int(wrenGetSlotDouble(vm, 3)),
        0, NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float)
        );
}

void plotNumberArrayWithOffset(WrenVM* vm) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign(vm, 2);
    ImGui::PlotLines(
        wrenGetSlotString(vm, 1), array->data(),
        int(wrenGetSlotDouble(vm, 3)), int(wrenGetSlotDouble(vm, 4)),
        NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float)
        );
}

void plotNumberArrayWithOffsetAndSize(WrenVM* vm) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign(vm, 2);
    ImGui::PlotLines(
        wrenGetSlotString(vm, 1), array->data(),
        int(wrenGetSlotDouble(vm, 3)), int(wrenGetSlotDouble(vm, 4)),
        NULL, FLT_MAX, FLT_MAX,
        *(const ImVec2*)wrenGetSlotForeign(vm, 5),
        sizeof(float)
        );
}

void plotRingBuffer(WrenVM* vm) {
    RingBuffer<float>* buffer = (RingBuffer<float>*)wrenGetSlotForeign(vm, 2);
    BasicStorage<float> numbers{ buffer->size() };
    for (unsigned int i = 0; i < buffer->size(); i++) {
        *numbers.at(i) = buffer->at(i);
    }
    ImGui::PlotLines(
        wrenGetSlotString(vm, 1), numbers.at(0),
        buffer->size(), 0,
        NULL, FLT_MAX, FLT_MAX,
        ImVec2(140, 80),
        sizeof(float)
        );
}

void plotRingBufferWithSize(WrenVM* vm) {
    RingBuffer<float>* buffer = (RingBuffer<float>*)wrenGetSlotForeign(vm, 2);
    BasicStorage<float> numbers{ buffer->size() };
    for (unsigned int i = 0; i < buffer->size(); i++) {
        *numbers.at(i) = buffer->at(i);
    }
    ImGui::PlotLines(
        wrenGetSlotString(vm, 1), numbers.at(0),
        buffer->size(), 0,
        NULL, FLT_MAX, FLT_MAX,
        *(const ImVec2*)wrenGetSlotForeign(vm, 3),
        sizeof(float)
        );
}

void setTitleBar(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits &= ~ImGuiWindowFlags_NoTitleBar;
}

void unsetTitleBar(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    (*bits) |= ImGuiWindowFlags_NoTitleBar;
}

void setResize(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits &= ~ImGuiWindowFlags_NoResize;
}

void setMove(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits &= ~ImGuiWindowFlags_NoMove;
}

void unsetMove(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits |= ImGuiWindowFlags_NoMove;
}

void unsetResize(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits |= ImGuiWindowFlags_NoResize;
}

void setShowBorders(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits |= ImGuiWindowFlags_ShowBorders;
}

void unsetShowBorders(WrenVM* vm) {
    uint32_t* bits = (uint32_t*)wrenGetSlotForeign(vm, 0);
    *bits &= ~ImGuiWindowFlags_ShowBorders;
}

/***
 *      ____            __               _
 *     / __ \__ _____ _/ /____ _______  (_)__  ___
 *    / /_/ / // / _ `/ __/ -_) __/ _ \/ / _ \/ _ \
 *    \___\_\_,_/\_,_/\__/\__/_/ /_//_/_/\___/_//_/
 *
 */

void conjugate(WrenVM* vm) {
    wrenEnsureSlots(vm, 1);
    const math::Quatf* q = (const math::Quatf*)wrenGetSlotForeign(vm, 0);
    math::Quatf res = q->conjugate();
    returnQuatValue(vm, res);
}

void inverse(WrenVM* vm) {
    wrenEnsureSlots(vm, 1);
    const math::Quatf* q = (const math::Quatf*)wrenGetSlotForeign(vm, 0);
    math::Quatf res = q->inverse();
    returnQuatValue(vm, res);
}

void multiply(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Quatf* lhs = (const math::Quatf*)wrenGetSlotForeign(vm, 0);
    const math::Quatf* rhs = (const math::Quatf*)wrenGetSlotForeign(vm, 1);
    math::Quatf res = lhs->multiply(*rhs);
    returnQuatValue(vm, res);
}

void axis(WrenVM* vm) {
    wrenEnsureSlots(vm, 1);
    const math::Quatf* q = (const math::Quatf*)wrenGetSlotForeign(vm, 0);
    math::Vec3f res = q->axis();
    returnVec3fValue(vm, res);
}

/***
 *       ___  _           __        ______
 *      / _ \(_)__  ___ _/ /  __ __/ _/ _/__ ____
 *     / , _/ / _ \/ _ `/ _ \/ // / _/ _/ -_) __/
 *    /_/|_/_/_//_/\_, /_.__/\_,_/_//_/ \__/_/
 *                /___/
 */

void ringBufferPushBack(WrenVM* vm) {
    RingBuffer<float>* ring = (RingBuffer<float>*)wrenGetSlotForeign(vm, 0);
    ring->pushBack(float(wrenGetSlotDouble(vm, 1)));
}

/***
 *      _   __        __
 *     | | / /__ ____/ /____  ____
 *     | |/ / -_) __/ __/ _ \/ __/
 *     |___/\__/\__/\__/\___/_/
 *
 */

void hadamard2f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec2f* lhs = (const math::Vec2f*)wrenGetSlotForeign(vm, 0);
    const math::Vec2f* rhs = (const math::Vec2f*)wrenGetSlotForeign(vm, 1);
    math::Vec2f res = lhs->hadamard(*rhs);
    returnVec2fValue(vm, res);
}

void plus2f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec2f* lhs = (const math::Vec2f*)wrenGetSlotForeign(vm, 0);
    const math::Vec2f* rhs = (const math::Vec2f*)wrenGetSlotForeign(vm, 1);
    math::Vec2f res = lhs->operator+(*rhs);
    returnVec2fValue(vm, res);
}

void minus2f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec2f* lhs = (const math::Vec2f*)wrenGetSlotForeign(vm, 0);
    const math::Vec2f* rhs = (const math::Vec2f*)wrenGetSlotForeign(vm, 1);
    math::Vec2f res = lhs->operator-(*rhs);
    returnVec2fValue(vm, res);
}

void cross3f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec3f* lhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 0);
    const math::Vec3f* rhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 1);
    math::Vec3f res = lhs->cross(*rhs);
    returnVec3fValue(vm, res);
}

void plus3f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec3f* lhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 0);
    const math::Vec3f* rhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 1);
    math::Vec3f res = lhs->operator+(*rhs);
    returnVec3fValue(vm, res);
}

void minus3f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec3f* lhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 0);
    const math::Vec3f* rhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 1);
    math::Vec3f res = lhs->operator-(*rhs);
    returnVec3fValue(vm, res);
}

void hadamard3f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec3f* lhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 0);
    const math::Vec3f* rhs = (const math::Vec3f*)wrenGetSlotForeign(vm, 1);
    math::Vec3f res = lhs->hadamard(*rhs);
    returnVec3fValue(vm, res);
}

void hadamard4f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec4f* lhs = (const math::Vec4f*)wrenGetSlotForeign(vm, 0);
    const math::Vec4f* rhs = (const math::Vec4f*)wrenGetSlotForeign(vm, 1);
    math::Vec4f res = lhs->hadamard(*rhs);
    returnVec4fValue(vm, res);
}

void plus4f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec4f* lhs = (const math::Vec4f*)wrenGetSlotForeign(vm, 0);
    const math::Vec4f* rhs = (const math::Vec4f*)wrenGetSlotForeign(vm, 1);
    math::Vec4f res = lhs->operator+(*rhs);
    returnVec4fValue(vm, res);
}

void minus4f(WrenVM* vm) {
    wrenEnsureSlots(vm, 2);
    const math::Vec4f* lhs = (const math::Vec4f*)wrenGetSlotForeign(vm, 0);
    const math::Vec4f* rhs = (const math::Vec4f*)wrenGetSlotForeign(vm, 1);
    math::Vec4f res = lhs->operator-(*rhs);
    returnVec4fValue(vm, res);
}

/***
*       ____         __
*      / __/_ _____ / /____ __ _  ___
*     _\ \/ // (_-</ __/ -_)  ' \(_-<
*    /___/\_, /___/\__/\__/_/_/_/___/
*        /___/
*/

// Pick3d
void castCameraRay(WrenVM* vm) {
    ecs::EntityManager* entities = Locator<ecs::EntityManager>::get();
    ecs::EventManager* events = Locator<ecs::EventManager>::get();
    system::PickingSystem* pick3d = Locator<system::PickingSystem>::get();
    int x = int(wrenGetSlotDouble(vm, 1));
    int y = int(wrenGetSlotDouble(vm, 2));
    ecs::Entity res = pick3d->rayCast(*entities, *events, x, y);
    wrenGetVariable(vm, "builtin/entity", "Entity", 0);
    //returnEntityValue(vm, res);
}

}   // wren
}   // pg
