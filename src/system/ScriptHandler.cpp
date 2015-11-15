#include "system/ScriptHandler.h"

namespace pg {
namespace system {

void ScriptHandler::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent< component::WrenScript> >( *this );
    events.subscribe< ecs::ComponentRemovedEvent< component::WrenScript > >( *this );
}

void ScriptHandler::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {
    for ( ecs::Entity entity: entities.join< component::WrenScript >() ) {
        entity.component< component::WrenScript >()->update();
     }
}

void ScriptHandler::receive( const ecs::ComponentAssignedEvent< component::WrenScript >& event ) {
    event.component->activate();
}

void ScriptHandler::receive( const ecs::ComponentRemovedEvent< component::WrenScript >& event ) {
    event.component->deactivate();
}

}
}