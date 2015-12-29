#include "component/Include.h"
#include "math/Vector.h"
#include "wren/WrenEntity.h"
#include "ecs/Include.h"
#include "utils/Locator.h"
#include <cstdint>

namespace pg {
namespace wren {

void set(WrenVM* vm) {
    ecs::Entity* e = (ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    uint32_t id = wrenGetArgumentDouble(vm, 1);
    ecs::EntityManager* entityManager = Locator< ecs::EntityManager >::get();
    *e = entityManager->get(id);
}

void entityIndex(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    wrenReturnDouble(vm, double(e->id().index()));
}

void entityVersion(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    wrenReturnDouble(vm, double(e->id().version()));
}

void setTransform(WrenVM* vm) {
    ecs::Entity* e = (ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    component::Transform* t = (component::Transform*)wrenGetArgumentForeign(vm, 1);
    *e->componentPointer< component::Transform >() = *t;
}

void hasTransform(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    wrenReturnBool(vm, e->has<component::Transform>());
}

void hasRenderable(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    wrenReturnBool(vm, e->has<component::Renderable>());
}

void hasCamera(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    wrenReturnBool(vm, e->has<component::Camera>());
}

void hasPointLight(WrenVM* vm) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign(vm, 0);
    wrenReturnBool(vm, e->has<component::PointLight>());
}

void createEntity(WrenVM* vm) {
    ecs::Entity entity = Locator<ecs::EntityManager>::get()->create();
    WrenValue* constructor = wrenGetMethod(vm, "builtin/entity", "createEntity", "call()");
    WrenValue* ret;
    wrenCall(vm, constructor, &ret, "");
}

void entityCount(WrenVM* vm) {
    wrenReturnDouble(vm, double(Locator<ecs::EntityManager>::get()->size()));
}

}
}
