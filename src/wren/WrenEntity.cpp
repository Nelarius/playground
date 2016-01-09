#include "app/MouseEvents.h"
#include "component/Include.h"
#include "math/Vector.h"
#include "wren/WrenEntity.h"
#include "ecs/Include.h"
#include "utils/Locator.h"
#include "utils/Assert.h"
#include "utils/Log.h"
#include "system/ScriptHandler.h"
#include <cstdint>

namespace pg {
namespace wren {

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

/***
*       ____     __  _ __       __  ___
*      / __/__  / /_(_) /___ __/  |/  /__ ____  ___ ____ ____ ____
*     / _// _ \/ __/ / __/ // / /|_/ / _ `/ _ \/ _ `/ _ `/ -_) __/
*    /___/_//_/\__/_/\__/\_, /_/  /_/\_,_/_//_/\_,_/\_, /\__/_/
*                       /___/                      /___/
*/

void createEntity(WrenVM* vm) {
    ecs::Entity entity = Locator<ecs::EntityManager>::get()->create();
    wrenSetSlotDouble(vm, 0, double(entity.id().index()));
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
    Locator<system::ScriptHandler>::get()->listenToKeyDown(wrenGetSlotString(vm, 2), entity);
}

void listenToKeyPressed(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptHandler>::get()->listenToKeyPressed(wrenGetSlotString(vm, 2), entity);
}

void listenToKeyUp(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptHandler>::get()->listenToKeyUp(wrenGetSlotString(vm, 2), entity);
}

void listenToMouseDown(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptHandler>::get()->listenToKeyDown(wrenGetSlotString(vm, 2), entity);
}

void listenToMousePressed(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptHandler>::get()->listenToMousePressed(wrenGetSlotString(vm, 2), entity);
}

void listenToMouseUp(WrenVM* vm) {
    ecs::Entity* entity = (ecs::Entity*)wrenGetSlotForeign(vm, 1);
    Locator<system::ScriptHandler>::get()->listenToMouseUp(wrenGetSlotString(vm, 2), entity);
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

}
}
