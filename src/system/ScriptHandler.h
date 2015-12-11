
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"
#include "system/Events.h"
#include "Wrenly.h"
#include <unordered_map>
#include <set>
#include <cstdlib>

namespace pg {

struct Context;

namespace system {

class ScriptHandler: public ecs::System<ScriptHandler>, public ecs::Receiver {
    public:
        ScriptHandler( Context& context );
        void configure( ecs::EventManager& ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;

        void receive( const ecs::ComponentAssignedEvent< component::Script >& );
        void receive( const ecs::ComponentRemovedEvent< component::Script >& );
        void receive( const system::TextFileUpdated& );

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

        Context&                context_;
        IdSet                   containedScripts_;
        std::set< std::size_t > updatedScripts_;
};

}   // system
}   // pg
