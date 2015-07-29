
#pragma once 

#include "utils/Pool.h"
#include "utils/Bundle.h"
#include "3rdparty/SimpleSignal.h"
#include "utils/Assert.h"
#include "utils/Log.h"
#include <unordered_map>    // faster key access than map, slower for iteration
#include <vector>
#include <cstdint>
#include <cstdlib>

using EventSignal = Simple::Signal<void(const void*)>;
using SignalBundle = ce::Bundle<EventSignal>;

namespace ce {
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

    private:
        friend class EventManager;
        
        inline void connect_( uint32_t family, std::size_t connection ) {
            connection_ = connection;
            connections_.insert( std::make_pair( family, true ) );
        }
        inline bool isConnected_( uint32_t family ) const {
            auto it = connections_.find( family );
            if ( it == connections_.end() ) {
                return false;
            }
            return it->second;
        }
        inline void disconnect_( uint32_t family ) {
            auto it = connections_.find( family );
            if ( it != connections_.end() ) {
                connections_.erase( it );
            }
            connection_ = 0u;
        }
        
        std::unordered_map<uint32_t, bool> connections_{};  // connections to event types
        std::size_t connection_{ 0u };
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
        SignalBundle  signals_{};
};

template<typename E, typename S>
void EventManager::subscribe( S& system ) {
    const unsigned family = Event<E>::family();
    ASSERT( !system.isConnected_( family ), "EventManager::subscribe> system already subscribed to event!" );
    accommodate_( family );
    std::size_t connection = signals_[family].connect( [&system]( const void* event ) -> void {
        system.receive( *(static_cast<const E*>( event )) );
    });
    system.connect_( family, connection );
}

template<typename E, typename S>
void EventManager::unsubscribe( S& system ) {
    const unsigned family = Event<E>::family();
    ASSERT( system.isConnected_( family ), "EventManager::unsubscribe> system not subscribed to event!" );
    signals_[family].disconnect( system.connection_ );
    system.disconnect( family );
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
