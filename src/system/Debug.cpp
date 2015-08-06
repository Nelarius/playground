#include "system/Debug.h"
#include "utils/Log.h"
#include <iostream>

namespace pg {
namespace system {

void Debug::configure( ecs::EventManager& events ) {
    events.subscribe<ecs::EntityDestroyedEvent>( *this );
    events.subscribe<ecs::EntityCreatedEvent>( *this );
    events.subscribe<ecs::ComponentAssignedEvent<component::Camera>>( *this );
    events.subscribe<ecs::ComponentAssignedEvent<component::Transform>>( *this );
    events.subscribe<ecs::ComponentAssignedEvent<component::Renderable>>( *this );
    events.subscribe<ecs::ComponentAssignedEvent<component::Script>>( *this );
    events.subscribe<ecs::ComponentRemovedEvent<component::Camera>>( *this );
    events.subscribe<ecs::ComponentRemovedEvent<component::Transform>>( *this );
    events.subscribe<ecs::ComponentRemovedEvent<component::Renderable>>( *this );
    events.subscribe<ecs::ComponentRemovedEvent<component::Script>>( *this );
}

void Debug::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {}

void Debug::receive( const ecs::EntityCreatedEvent& event ) {
	LOG_INFO << "Entity " << event.entity.id().index() << "." << event.entity.id().version() << " created.";
}

void Debug::receive( const ecs::EntityDestroyedEvent& event ) {
	LOG_INFO << "Entity " << event.entity.id().index() << "." << event.entity.id().version() << " destroyed.";
}

void Debug::receive( const ecs::ComponentAssignedEvent<component::Camera>& event ) {
	LOG_INFO << "Camera component assigned to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentAssignedEvent<component::Transform>& event ) {
	LOG_INFO << "Transform component assigned to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentAssignedEvent<component::Renderable>& event ) {
	LOG_INFO << "Renderable component assigned to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentAssignedEvent<component::Script>& event ) {
    LOG_INFO << "Script component assigned to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentRemovedEvent<component::Camera>& event ) {
	LOG_INFO << "Camera component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentRemovedEvent<component::Transform>& event ) {
	LOG_INFO << "Transform component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentRemovedEvent<component::Renderable>& event ) {
	LOG_INFO << "Renderable component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ecs::ComponentRemovedEvent<component::Script>& event ) {
    LOG_INFO << "Script component removed Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

}   // system
}   // pg
