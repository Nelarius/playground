#include "app/PauseState.h"

namespace pg {

PauseState::PauseState( Context& context, AppStateStack& stack )
:   AppState( context, stack )
    {}

void PauseState::activate() {
    keyboard_.registerKeyDownCommand( Keycode::KeyP, Command(
        [ this ]() -> void {
          this->requestStackPop_();
      },
      std::function<void()>()
    ) );
}

bool PauseState::update( float dt ) {
    return false;
}

bool PauseState::handleEvent( const SDL_Event& event ) {
    if ( event.type == SDL_QUIT ) {
        requestStackClear_();
        context_.running = false;
    }
    keyboard_.handleEvent( event );
    return false;
}

void PauseState::render( float dt ) {
    // do nothing
}

}