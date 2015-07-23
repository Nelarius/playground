
#pragma once

#include <entityx/entityx.h>
#include "component/Components.h"

namespace ex = entityx;

namespace ce {
namespace system {

/*
 * This has to be a struct, or else the compiler can't find BaseReceiver...
 * */
struct Debug : public ex::System<Debug>, ex::Receiver<Debug> {
	public:	
		void configure( ex::EventManager& events ) override;
		void update( ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt ) override;
		void receive( const ex::EntityCreatedEvent& created );
		void receive( const ex::EntityDestroyedEvent& destroyed );
		void receive( const ex::ComponentAddedEvent<component::Camera>& camera );
		void receive( const ex::ComponentAddedEvent<component::Transform>& transform );
		void receive( const ex::ComponentAddedEvent<component::Renderable>& renderable );
        void receive( const ex::ComponentAddedEvent<component::Body>& body );
		void receive( const ex::ComponentRemovedEvent<component::Camera>& camera );
		void receive( const ex::ComponentRemovedEvent<component::Transform>& transform );
		void receive( const ex::ComponentRemovedEvent<component::Renderable>& renderable );
        void receive( const ex::ComponentRemovedEvent<component::Body>& body );
};


}
}

