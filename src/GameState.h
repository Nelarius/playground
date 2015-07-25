
#pragma once

#include "app/AppState.h"
#include "app/Context.h"
#include <entityx/entityx.h>
#include "manager/ShaderManager.h"

namespace ex = entityx;

union SDL_Event;

namespace ce {

class AppStateStack;

/*
 * EntityManager can't be copied due to the component pools contained and
 * other ownership issues. Thus this class shouldn't be copied either.
 * 
 * AppStates are created on the heap, so this isn't a problem.
 * */
class GameState : public AppState {
    public:

        GameState( Context& context, AppStateStack& stack );
        GameState()                                 = delete;
        GameState( const GameState& )               = default;
        GameState( GameState&& )                    = default;
        GameState& operator=( const GameState& )    = default;
        GameState& operator=( GameState&& )         = default;
        virtual ~GameState()                        = default;
        
        void activate() override;
        void render( float dt ) override;
        bool update( float dt ) override;
        bool handleEvent( const SDL_Event& event ) override;
    
    private:
        // METHODS
        void parseScene_();
        
        // FIELDS
        // these could be possibly moved to AppState
        ex::EventManager    events_{};
        ex::EntityManager   entities_{ events_ };
        ex::SystemManager   systems_{ entities_, events_ };        
};

}

