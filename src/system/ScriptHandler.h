
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"
#include "Wrenly.h"
#include <unordered_map>
#include <cstdlib>

namespace pg {
namespace system {

class ScriptHandler: public ecs::System<ScriptHandler>, public ecs::Receiver {
    public:
        void configure( ecs::EventManager& ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;

        void receive( const ecs::ComponentAssignedEvent< component::Script >& );
        void receive( const ecs::ComponentRemovedEvent< component::Script >& );

    private:
        // used to keep track of which scripts are currently in use
        class IdSet {
            public:
                IdSet() = default;
                ~IdSet() = default;

                void add( std::size_t );
                bool contains( std::size_t ) const;
                void remove( std::size_t );

            private:
                std::unordered_map< std::size_t, int > ids_{};
        };

        IdSet containedScripts_{};
};

}   // system
}   // pg
