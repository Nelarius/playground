#include "system/ScriptHandler.h"
#include "utils/Assert.h"

namespace pg {
namespace system {

void ScriptHandler::IdSet::add( std::size_t id ) {
    auto it = ids_.find( id );
    if ( it != ids_.end() ) {
        it->second++;
    } else {
        ids_.insert( std::make_pair( id, 1 ) );
    }
}

bool ScriptHandler::IdSet::contains( std::size_t id ) const {
    auto it = ids_.find( id );
    if ( it != ids_.end() ) {
        return true;
    }
    return false;
}

void ScriptHandler::IdSet::remove( std::size_t id ) {
    auto it = ids_.find( id );
    ASSERT( it != ids_.end() );
    it->second--;
    if ( it->second == 0 ) {
        ids_.erase( it );
    }
}

void ScriptHandler::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent< component::Script> >( *this );
    events.subscribe< ecs::ComponentRemovedEvent< component::Script > >( *this );
}

void ScriptHandler::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {
    for ( ecs::Entity entity: entities.join< component::Script >() ) {
        entity.component< component::Script >()->update( dt );
     }
}

void ScriptHandler::receive( const ecs::ComponentAssignedEvent< component::Script >& event ) {
    event.component->activate();
    containedScripts_.add( event.component->scriptId );
}

void ScriptHandler::receive( const ecs::ComponentRemovedEvent< component::Script >& event ) {
    event.component->deactivate();
    containedScripts_.remove( event.component->scriptId );
}

}
}
