
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"

namespace pg {
namespace system {

class Debug : public ecs::System<Debug>, public ecs::Receiver {
    public:	
        void configure( ecs::EventManager& events ) override;
        void update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) override;
        void receive( const ecs::EntityCreatedEvent& created );
        void receive( const ecs::EntityDestroyedEvent& destroyed );
        void receive( const ecs::ComponentAssignedEvent<component::Camera>& camera );
        void receive( const ecs::ComponentAssignedEvent<component::Transform>& transform );
        void receive( const ecs::ComponentAssignedEvent<component::Renderable>& renderable );
        void receive( const ecs::ComponentAssignedEvent<component::Script>& script );
        void receive( const ecs::ComponentRemovedEvent<component::Camera>& camera );
        void receive( const ecs::ComponentRemovedEvent<component::Transform>& transform );
        void receive( const ecs::ComponentRemovedEvent<component::Renderable>& renderable );
        void receive( const ecs::ComponentRemovedEvent<component::Script>& script );
};


}
}

