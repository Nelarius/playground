#include "system/WrenSystem.h"

namespace pg {
namespace system {

void WrenSystem::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent< component::WrenScript> >( *this );
    events.subscribe< ecs::ComponentRemovedEvent< component::WrenScript > >( *this );
}

void WrenSystem::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {
    for ( ecs::Entity entity: entities.join< component::WrenScript >() ) {
        entity.component< component::WrenScript >()->update();
     }
}

void WrenSystem::receive( const ecs::ComponentAssignedEvent< component::WrenScript >& event ) {
    event.component->activate();
}

void WrenSystem::receive( const ecs::ComponentRemovedEvent< component::WrenScript >& event ) {
    event.component->deactivate();
}
   
}
}