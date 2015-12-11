
#pragma once

#include "ecs/Include.h"
#include "app/AppState.h"
#include "app/Context.h"
#include "manager/ShaderManager.h"

union SDL_Event;

namespace pg {

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
        GameState( const GameState& )               = delete;
        GameState( GameState&& )                    = delete;
        GameState& operator=( const GameState& )    = delete;
        GameState& operator=( GameState&& )         = delete;
        virtual ~GameState()                        = default;

        void activate() override;
        void render( float dt ) override;
        bool update( float dt ) override;
        bool handleEvent( const SDL_Event& event ) override;

    private:
        // FIELDS
        // these could be possibly moved to AppState
        ecs::EventManager   events_;
        ecs::EntityManager  entities_;
        ecs::SystemManager  systems_;
        //KeyboardManager      keyboard_;
        //MouseEvents          mouse_;
};

}

