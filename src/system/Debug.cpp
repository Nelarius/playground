#include "system/Debug.h"
#include "utils/Log.h"
#include <iostream>

using ce::system::Debug;

void Debug::configure( ecs::EventManager& events ) {
    events.subscribe<ce::ecs::EntityDestroyedEvent>( *this );
    events.subscribe<ce::ecs::EntityCreatedEvent>( *this );
    events.subscribe<ce::ecs::ComponentAssignedEvent<ce::component::Camera>>( *this );
    events.subscribe<ce::ecs::ComponentAssignedEvent<ce::component::Transform>>( *this );
    events.subscribe<ce::ecs::ComponentAssignedEvent<ce::component::Renderable>>( *this );
    events.subscribe<ce::ecs::ComponentRemovedEvent<ce::component::Camera>>( *this );
    events.subscribe<ce::ecs::ComponentRemovedEvent<ce::component::Transform>>( *this );
    events.subscribe<ce::ecs::ComponentRemovedEvent<ce::component::Renderable>>( *this );
}

void Debug::update( ce::ecs::EntityManager& entities, ce::ecs::EventManager& events, float dt ) {}

void Debug::receive( const ce::ecs::EntityCreatedEvent& event ) {
	LOG_INFO << "Entity " << event.entity.id().index() << "." << event.entity.id().version() << " created.";
}

void Debug::receive( const ce::ecs::EntityDestroyedEvent& event ) {
	LOG_INFO << "Entity " << event.entity.id().index() << "." << event.entity.id().version() << " destroyed.";
}

void Debug::receive( const ce::ecs::ComponentAssignedEvent<ce::component::Camera>& event ) {
	LOG_INFO << "Camera component added to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ce::ecs::ComponentAssignedEvent<ce::component::Transform>& event ) {
	LOG_INFO << "Transform component added to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ce::ecs::ComponentAssignedEvent<ce::component::Renderable>& event ) {
	LOG_INFO << "Renderable component added to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ce::ecs::ComponentRemovedEvent<ce::component::Camera>& event ) {
	LOG_INFO << "Camera component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ce::ecs::ComponentRemovedEvent<ce::component::Transform>& event ) {
	LOG_INFO << "Transform component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ce::ecs::ComponentRemovedEvent<ce::component::Renderable>& event ) {
	LOG_INFO << "Renderable component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}



