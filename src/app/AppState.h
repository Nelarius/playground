
#pragma once

#include "app/Context.h"
#include "StateIdentifiers.h"
#include "app/KeyboardManager.h"
#include "app/MouseEvents.h"

namespace pg {

class AppStateStack;

class AppState {
    public:
        AppState() = delete;
        AppState( Context& context, AppStateStack& stack );
        virtual ~AppState() = default;
        
        virtual void activate()                             = 0;
        virtual void render( float dt )                     = 0;
        virtual bool update( float dt )                     = 0;
        virtual bool handleEvent( const SDL_Event& event )  = 0;

    protected:
        // METHODS
        void requestStackPush_( states::Id id );	// add state ID
        void requestStackPop_();
        void requestStackClear_();
        // FIELDS
        Context&            context_;
        KeyboardManager     keyboard_;
        MouseEvents         mouse_;

    private:
        AppStateStack&	stack_;
};

}
