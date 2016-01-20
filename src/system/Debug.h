#pragma once

#include "ecs/Include.h"

namespace pg {

namespace math {
struct AABox;
}

namespace component {
struct Camera;
struct Transform;
struct Renderable;
struct PointLight;
struct Script;
}

namespace system {

class Debug : public ecs::System<Debug>, public ecs::Receiver {
    public:	
        void configure( ecs::EventManager& events ) override;
        void update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) override;
        void receive( const ecs::EntityCreatedEvent& created );
        void receive( const ecs::EntityDestroyedEvent& destroyed );
        void receive( const ecs::ComponentAssignedEvent< component::Camera >& );
        void receive( const ecs::ComponentAssignedEvent< component::Transform >& );
        void receive( const ecs::ComponentAssignedEvent< component::Renderable >& );
        void receive( const ecs::ComponentAssignedEvent< math::AABox >& );
        void receive( const ecs::ComponentAssignedEvent< component::PointLight >& );
        void receive( const ecs::ComponentAssignedEvent< component::Script >& );

        void receive( const ecs::ComponentRemovedEvent< component::Camera >& );
        void receive( const ecs::ComponentRemovedEvent< component::Transform >& );
        void receive( const ecs::ComponentRemovedEvent< component::Renderable >& );
        void receive( const ecs::ComponentRemovedEvent< math::AABox >& );
        void receive( const ecs::ComponentRemovedEvent< component::PointLight >& );
        void receive( const ecs::ComponentRemovedEvent< component::Script >& );
};


}
}
