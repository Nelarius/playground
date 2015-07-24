#include "system/Debug.h"
#include "utils/Log.h"
#include <iostream>

using ce::system::Debug;
using ce::LogLevel;

void Debug::configure( ex::EventManager& events ) {
    events.subscribe<ex::EntityDestroyedEvent>( *this );
    events.subscribe<ex::EntityCreatedEvent>( *this );
    events.subscribe<ex::ComponentAddedEvent<ce::component::Camera>>( *this );
    events.subscribe<ex::ComponentAddedEvent<ce::component::Transform>>( *this );
    events.subscribe<ex::ComponentAddedEvent<ce::component::Renderable>>( *this );
    events.subscribe<ex::ComponentRemovedEvent<ce::component::Camera>>( *this );
    events.subscribe<ex::ComponentRemovedEvent<ce::component::Transform>>( *this );
    events.subscribe<ex::ComponentRemovedEvent<ce::component::Renderable>>( *this );
}

void Debug::update( ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt ) {
	//
}

void Debug::receive( const ex::EntityCreatedEvent& event ) {
	LOG( LogLevel::Info ) << "Entity " << event.entity.id().index() << "." << event.entity.id().version() << " created.";
}

void Debug::receive( const ex::EntityDestroyedEvent& event ) {
	LOG( LogLevel::Info ) << "Entity " << event.entity.id().index() << "." << event.entity.id().version() << " destroyed.";
}

void Debug::receive( const ex::ComponentAddedEvent<ce::component::Camera>& event ) {
	LOG( LogLevel::Info ) << "Camera component added to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ex::ComponentAddedEvent<ce::component::Transform>& event ) {
	LOG( LogLevel::Info ) << "Transform component added to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ex::ComponentAddedEvent<ce::component::Renderable>& event ) {
	LOG( LogLevel::Info ) << "Renderable component added to Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ex::ComponentRemovedEvent<ce::component::Camera>& event ) {
	LOG( LogLevel::Info ) << "Camera component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ex::ComponentRemovedEvent<ce::component::Transform>& event ) {
	LOG( LogLevel::Info ) << "Transform component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}

void Debug::receive( const ex::ComponentRemovedEvent<ce::component::Renderable>& event ) {
	LOG( LogLevel::Info ) << "Renderable component removed from Entity " << event.entity.id().index() << "." << event.entity.id().version();
}


