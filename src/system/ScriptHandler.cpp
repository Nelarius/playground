#include "system/ScriptHandler.h"
#include "app/Context.h"
#include "utils/Assert.h"

namespace pg {
namespace system {

void ScriptHandler::IdSet::add( std::size_t id ) {
    auto it = ids_.find( id );
    if ( it != ids_.end() ) {
        it->second++;
    } else {
        ids_.insert( std::make_pair( id, 1 ) );
    }
}

bool ScriptHandler::IdSet::contains( std::size_t id ) const {
    auto it = ids_.find( id );
    if ( it != ids_.end() ) {
        return true;
    }
    return false;
}

void ScriptHandler::IdSet::remove( std::size_t id ) {
    auto it = ids_.find( id );
    ASSERT( it != ids_.end() );
    it->second--;
    if ( it->second == 0 ) {
        ids_.erase( it );
    }
}

ScriptHandler::ScriptHandler( Context& context )
:   context_{ context },
    containedScripts_{},
    updatedScripts_{}
    {}

void ScriptHandler::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent< component::Script> >( *this );
    events.subscribe< ecs::ComponentRemovedEvent< component::Script > >( *this );
    events.subscribe< system::TextFileUpdated >( *this );
}

void ScriptHandler::update( ecs::EntityManager& entities, ecs::EventManager& events, float dt ) {
    if ( !updatedScripts_.empty() ) {
        for ( auto entity: entities.join< component::Script>() ) {
            for ( auto id: updatedScripts_ ) {
                if ( id != entity.component<component::Script>()->scriptId ) {
                    continue;
                }

                wrenly::Wren vm;
                vm.executeString(
                    "import \"builtin/entity\" for Entity\n"
                    "var entity = Entity.new()\n"
                );
                auto set = vm.method( "main", "entity", "set(_)" );
                set( int(entity.id().index()) );
                wrenly::Result res = vm.executeString( context_.textFileManager.get( id ) );
                if ( res != wrenly::Result::Success ) {
                    continue;
                }
                entity.remove< component::Script >();
                auto activate = vm.method( "main", "activate", "call()" );
                auto deactivate = vm.method( "main", "deactivate", "call()" );
                auto update = vm.method( "main", "update", "call(_)" );
                entity.assign< component::Script >(
                    id,
                    std::move( vm ),
                    activate,
                    deactivate,
                    update
                );
            }
        }
        updatedScripts_.clear();
    }

    for ( ecs::Entity entity: entities.join< component::Script >() ) {
        entity.component< component::Script >()->update( dt );
     }
}

void ScriptHandler::receive( const ecs::ComponentAssignedEvent< component::Script >& event ) {
    event.component->activate();
    containedScripts_.add( event.component->scriptId );
}

void ScriptHandler::receive( const ecs::ComponentRemovedEvent< component::Script >& event ) {
    event.component->deactivate();
    containedScripts_.remove( event.component->scriptId );
}

void ScriptHandler::receive( const system::TextFileUpdated& event ) {
    LOG_DEBUG << "Text file (id: " << event.id << ") updated";
    if ( containedScripts_.contains( event.id ) ) {
        updatedScripts_.insert( event.id );
    }
}

}
}
