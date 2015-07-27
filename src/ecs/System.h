
#pragma once

#include "ecs/Component.h"
#include "ecs/Event.h"
#include "ecs/Entity.h"
#include "utils/Assert.h"
#include <unordered_map>
#include <memory>
#include <cstddef>  // for std::size_t
#include <cstdint>

namespace ce {
namespace ecs {

class BaseSystem {
    public:
        BaseSystem()            = default;
        virtual ~BaseSystem()    = default;
        
        virtual void configure() = 0;
        virtual void update( float dt ) = 0;
    protected:
        static uint32_t familyCounter_;
};

template<typename S>
class System: public BaseSystem {
    public:
        System()            = default;
        virtual ~System()   = default;
    
        static uint32_t family() {
            static uint32_t f{ familyCounter_++ };
            return f;
        }
};

class SystemManager {
    public:
        SystemManager() = default;
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
        std::unordered_map<uint32_t, std::shared_ptr<BaseSystem>>   systems_{};
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
    ASSERT( it != systems_.end(), "SystemManager::system> system not added yet!" );
    std::shared_ptr<S>( std::static_pointer_cast<S>( it->second ) );
}

template<typename S>
void SystemManager::configure() {
    auto it = systems_.find( S::family() );
    ASSERT( it != systems_.end(), "SystemManager::configure> system not added yet!" );
    it->configure();
}

template<typename S>
void SystemManager::update( float dt ) {
    auto it = systems_.find( S::family() );
    ASSERT( it != systems_.end(), "SystemManager::update> system not added yet!" );
    it->update( dt );
}

}   // namespace ecs
}   // namespace ce 

