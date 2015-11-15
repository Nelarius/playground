
#pragma once

#include "ecs/Include.h"
#include "component/WrenScript.h"
#include "Wrenly.h"

namespace pg {
namespace system {

class ScriptHandler: public ecs::System<ScriptHandler>, public ecs::Receiver {
    public:
        void configure( ecs::EventManager& ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;

        void receive( const ecs::ComponentAssignedEvent< component::WrenScript >& );

        void receive( const ecs::ComponentRemovedEvent< component::WrenScript >& );

    private:
};

}   // system
}   // pg
