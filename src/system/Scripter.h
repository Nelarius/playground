
#pragma once

#include "ecs/Include.h"
#include "component/Script.h"

namespace pg {
namespace system {

class Scripter: public ecs::System<Scripter>, public ecs::Receiver {
    public:
        void configure( ecs::EventManager& ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;
        
        void receive( const ecs::ComponentAssignedEvent<pg::component::Script>& );
        void receive( const ecs::ComponentRemovedEvent<pg::component::Script>& );
};
    

}   // system
}   // pg
