
#pragma once

#include "ecs/Component.h"
#include "ecs/Event.h"
#include "ecs/Entity.h"
#include "utils/Assert.h"
#include <unordered_map>
#include <memory>
#include <cstddef>  // for std::size_t
#include <cstdint>

namespace pg {
namespace ecs {
    
class EntityManager;
class EventManager;

class BaseSystem {
    public:
        BaseSystem()            = default;
        virtual ~BaseSystem()    = default;
        
        virtual void configure( EventManager& ) = 0;
        virtual void update( EntityManager&, EventManager&, float dt ) = 0;
    protected:
        static uint32_t familyCounter_;
};

template<typename S>
class System: public BaseSystem {
    public:
        System()            = default;
        virtual ~System()   = default;
        
        virtual void configure( EventManager& ) {}
        virtual void update( EntityManager&, EventManager&, float dt ) {}
    
        static uint32_t family() {
            static uint32_t f{ familyCounter_++ };
            return f;
        }
};

class SystemManager {
    public:
        SystemManager( EventManager&, EntityManager& );
        ~SystemManager() = default;
        
        template<typename S, typename... Args>
        std::shared_ptr<S> add( Args&&... args);
        
        template<typename S>
        std::shared_ptr<S> system();
        
        template<typename S>
        void configure();
        template<typename S>
        void update( float dt );
    
    private:
        EventManager&   events_;
        EntityManager&  entities_;
        std::unordered_map<uint32_t, std::shared_ptr<BaseSystem>>   systems_;
};

/////////////////////////////////////////////////////////////////////////////
// SystemManager implementation
/////////////////////////////////////////////////////////////////////////////
template<typename S, typename... Args>
std::shared_ptr<S> SystemManager::add( Args&&... args ) {
    std::shared_ptr<S> s( new S( std::forward<Args>( args )... ) );
    systems_.insert( std::make_pair( S::family(), s ) );
    return s;
}

template<typename S>
std::shared_ptr<S> SystemManager::system() {
    auto it = systems_.find( S::family() );
    ASSERT( it != systems_.end() );
    std::shared_ptr<S>( std::static_pointer_cast<S>( it->second ) );
}

template<typename S>
void SystemManager::configure() {
    auto it = systems_.find( S::family() );
    ASSERT( it != systems_.end() );
    it->second->configure( events_ );
}

template<typename S>
void SystemManager::update( float dt ) {
    auto it = systems_.find( S::family() );
    ASSERT( it != systems_.end() );
    it->second->update( entities_, events_, dt );
}

}   // namespace ecs
}   // namespace ce 

