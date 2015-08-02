
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"

namespace ce {
namespace system {

class Debug : public ce::ecs::System<Debug>, public ce::ecs::Receiver {
    public:	
        void configure( ce::ecs::EventManager& events ) override;
        void update( ce::ecs::EntityManager& entities, ce::ecs::EventManager& events, float dt ) override;
        void receive( const ce::ecs::EntityCreatedEvent& created );
        void receive( const ce::ecs::EntityDestroyedEvent& destroyed );
        void receive( const ce::ecs::ComponentAssignedEvent<component::Camera>& camera );
        void receive( const ce::ecs::ComponentAssignedEvent<component::Transform>& transform );
        void receive( const ce::ecs::ComponentAssignedEvent<component::Renderable>& renderable );
        void receive( const ce::ecs::ComponentRemovedEvent<component::Camera>& camera );
        void receive( const ce::ecs::ComponentRemovedEvent<component::Transform>& transform );
        void receive( const ce::ecs::ComponentRemovedEvent<component::Renderable>& renderable );
};


}
}

