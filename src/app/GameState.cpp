#include "app/GameState.h"
#include "app/AppStateStack.h"
#include "app/WorldIO.h"
#include "system/Debug.h"
#include "system/Renderer.h"
#include "system/ScriptHandler.h"
#include "system/Ui.h"
#include "system/Events.h"
#include "utils/Locator.h"

#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

namespace pg {

GameState::GameState( Context& context, AppStateStack& stack ) 
:   AppState( context, stack ),
    events_(),
    entities_( events_ ),
    systems_( events_, entities_ ) {
    Locator< ecs::EntityManager >::set( &entities_ );
    Locator< ecs::EventManager >::set ( &events_ );
}

void GameState::activate() {
    systems_.add< system::Renderer >( context_ );
    systems_.add< system::Debug >();
    systems_.add< system::ScriptHandler >( context_ );
    systems_.add< system::Ui >( context_ );
    systems_.configure< system::Debug >();
    systems_.configure< system::Renderer >();
    systems_.configure< system::ScriptHandler >();

    // the full capacity of the systems are used in parsing, so the systems must be configured and ready to go!
    WorldIO world( context_ );
    world.read( "data/scene.json", entities_, events_ );

    keyboard_.registerKeyDownCommand( Keycode::KeyF1, Command(
      [ this ]() -> void {
          auto ui = this->systems_.system< system::Ui >();
          ui->toggleDisplay();
      },
      std::function<void()>()
    ) );
    keyboard_.registerKeyDownCommand( Keycode::KeyP, Command(
        [ this ]() -> void {
          this->requestStackPush_( states::Pause );
      },
      std::function<void()>()
    ) );
}

bool GameState::update( float dt ) {
    keyboard_.handleKeyPressedCommands();
    mouse_.update();
    systems_.update< system::ScriptHandler >( dt );
    return false;
}

bool GameState::handleEvent( const SDL_Event& event ) { 
    if ( event.type == SDL_QUIT ) {
        requestStackClear_();
        context_.running = false;
    }
    keyboard_.handleEvent( event );
    return false; 
}

void GameState::render( float dt ) {
    systems_.update< system::Renderer >( dt );
    systems_.update< system::Ui >( dt );
}

}
