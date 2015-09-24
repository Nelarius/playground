
#pragma once 

#include "utils/Pool.h"
#include "utils/Container.h"
#include "3rdparty/SimpleSignal.h"
#include "utils/Assert.h"
#include "utils/Log.h"
#include <unordered_map>    // faster key access than map, slower for iteration
#include <vector>
#include <cstdint>
#include <cstdlib>

using EventSignal = Simple::Signal<void(const void*)>;
using SignalContainer = pg::Container<EventSignal>;

namespace pg {
namespace ecs {

class BaseEvent {
    protected:
        static uint32_t familyCounter_;
};
    
template<typename E>
class Event: public BaseEvent {
    public:
        static uint32_t family() {
            static uint32_t f{ familyCounter_++ };
            return f;
        }
};

class EventManager;

class Receiver {
    public:
        Receiver()          = default;
        virtual ~Receiver() = default;
    
        friend class EventManager;
        
    private:
        inline void connect_( uint32_t family, std::size_t connection ) {
            connections_.insert( std::make_pair( family, connection ) );
        }
        inline bool isConnected_( uint32_t family ) const {
            auto it = connections_.find( family );
            if ( it == connections_.end() ) {
                return false;
            }
            return true;
        }
        inline uint32_t connection_( uint32_t family ) const {
            auto it = connections_.find( family );
            ASSERT( it != connections_.end() );
            return it->second;
        }
        inline void disconnect_( uint32_t family ) {
            auto it = connections_.find( family );
            if ( it != connections_.end() ) {
                connections_.erase( it );
            }
        }
        
        std::unordered_map<uint32_t, std::size_t> connections_{};  // connections to event types
};

class EventManager {
    public:
        EventManager() = default;
        ~EventManager() = default;
        
        template<typename E, typename S>
        void subscribe( S& system );
        
        template<typename E, typename S>
        void unsubscribe( S& system );
        
        template<typename E, typename... Args>
        void emit( Args&&... args );
        
    private:
        inline void accommodate_( uint32_t family ) {
            while ( family >= signals_.size() ) {
                signals_.emplace();
            }
        }
        SignalContainer  signals_{};
};

template<typename E, typename S>
void EventManager::subscribe( S& system ) {
    Receiver& receiver = static_cast<Receiver&>( system );
    const uint32_t family = Event<E>::family();
    ASSERT( !receiver.isConnected_( family ) );
    accommodate_( family );
    std::size_t connection = signals_[family].connect( [&system]( const void* event ) -> void {
        system.receive( *(static_cast<const E*>( event )) );
    });
    receiver.connect_( family, connection );
}

template<typename E, typename S>
void EventManager::unsubscribe( S& system ) {
    Receiver& receiver = static_cast<Receiver&> (system);
    const uint32_t family = Event<E>::family();
    ASSERT( receiver.isConnected_( family ) );
    signals_[family].disconnect( receiver.connection_( family ) );
    receiver.disconnect_( family );
}

template<typename E, typename... Args>
void EventManager::emit( Args&&... args ) {
    const unsigned family = Event<E>::family();
    accommodate_( family );
    E event{ std::forward<Args>( args )... };
    signals_[family].emit( &event );
}

}   // ecs
}   // ce
