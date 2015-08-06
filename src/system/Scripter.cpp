#include "system/Scripter.h"
#include "utils/Log.h"
#include "lua/LuaBridge.h"

#include <iostream>

namespace pg {
namespace system {

void Scripter::configure( ecs::EventManager& events ) {
    events.subscribe<ecs::ComponentAssignedEvent<component::Script>>( *this );
    events.subscribe<ecs::ComponentRemovedEvent<component::Script>>( *this );
}

void Scripter::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {
    /*for ( ecs::Entity entity: entities.join<component::Script>() ) {
        lb::LuaRef upadte = lb::getGlobal( entity.component<component::Script>()->state.get(), "update" );
        if ( upadte ) {
            upadte( dt );
        }
    }*/
}

void Scripter::receive( const ecs::ComponentAssignedEvent<component::Script>& assigned ) {
    lb::LuaRef activate = lb::getGlobal( assigned.component->state.get(), "activate" );
    if ( activate ) {
        activate();
    }
}

void Scripter::receive( const ecs::ComponentRemovedEvent<component::Script>& removed ) {
    lb::LuaRef deactivate = lb::getGlobal( removed.component->state.get(), "deactivate" );
    if ( deactivate ) {
        deactivate();
    }
}

}
}
