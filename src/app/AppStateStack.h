
#pragma once

#include "app/AppState.h"
#include "app/Context.h"
#include "StateIdentifiers.h"
#include <SDL2/SDL_events.h>
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <functional>

namespace pg {
    
namespace states {
    enum Action {
        Push,
        Pop,
        Clear
    };
}

class AppStateStack {
public:
    explicit AppStateStack( Context& context );
    ~AppStateStack() = default;

    template<typename T>
    void registerState( states::Id id );

    void render( float dt );
    void update( float dt );
    void handleEvent( const SDL_Event& event );

    void pushState( states::Id id );
    void popState();
    void clearStates();
    
    bool isEmpty() const;

    private:
        // METHODS
        std::unique_ptr<AppState> 	createState_( states::Id id );
        void 						applyPendingChanges_();

        // STRUCTS
        struct PendingChange {
            explicit PendingChange( states::Action action, states::Id id = states::Id::None );
            
            states::Action action;
            states::Id id;
        };

        // FIELDS
        Context&	context_;
        std::vector<std::unique_ptr<AppState>>	stack_;
        std::vector<PendingChange>				pendingList_;
        std::map<states::Id, std::function<std::unique_ptr<AppState>()>>	factories_;

};

template<typename T>
void AppStateStack::registerState( states::Id id ) {
    factories_.insert( std::make_pair( id, [this](){
        return std::unique_ptr<AppState>( new T( context_, *this) );
    } ) );
}

}