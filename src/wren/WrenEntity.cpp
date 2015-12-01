#include "wren/WrenEntity.h"
#include "ecs/Include.h"
#include "component/Include.h"
#include "utils/Locator.h"
#include <cstdint>

namespace pg {
namespace wren {

void set( WrenVM* vm ) {
    ecs::Entity* e = (ecs::Entity*)wrenGetArgumentForeign( vm, 0 );
    uint32_t id = wrenGetArgumentDouble( vm, 1 );
    ecs::EntityManager* entityManager = Locator< ecs::EntityManager >::get();
    *e = entityManager->get( id );
}

void entityIndex( WrenVM* vm ) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign( vm, 0 );
    wrenReturnDouble( vm, double(e->id().index()) );
}

void entityVersion( WrenVM* vm ) {
    const ecs::Entity* e = (const ecs::Entity*)wrenGetArgumentForeign( vm, 0 );
    wrenReturnDouble( vm, double(e->id().version()) );
}

void setTransform( WrenVM* vm ) {
    ecs::Entity* e = (ecs::Entity*)wrenGetArgumentForeign( vm, 0 );
    component::Transform* t = (component::Transform*)wrenGetArgumentForeign( vm, 1 );
    *e->componentPointer< component::Transform >() = *t;
}

}
}