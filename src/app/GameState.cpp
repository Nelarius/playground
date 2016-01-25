#include "app/GameState.h"
#include "app/AppStateStack.h"
#include "app/WorldIO.h"
#include "system/Debug.h"
#include "system/Renderer.h"
#include "system/ScriptHandler.h"
#include "system/Ui.h"
#include "system/Events.h"
#include "system/PickingSystem.h"
#include "system/DebugRenderSystem.h"
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
    Locator<MouseEvents>::set(&mouse_); // the Wren script needs these
}

void GameState::activate() {
    systems_.add< system::Renderer >( context_ );
    systems_.add<system::DebugRenderSystem>( context_);
    systems_.add< system::PickingSystem >(context_);
    systems_.add< system::Debug >();
    systems_.add< system::ScriptHandler >( context_, keyboard_, mouse_ );
    systems_.add< system::Ui >( context_ );
    systems_.configure< system::Debug >();
    systems_.configure< system::Renderer >();
    systems_.configure<system::DebugRenderSystem>();
    systems_.configure<system::PickingSystem>();
    systems_.configure< system::ScriptHandler >();

    // NOTICE
    // this is a dirty hack to get ScriptHandler bound to Wren
    // I really need a way to set the bytes of a foreign object in a better way....
    Locator<system::ScriptHandler>::set(systems_.system<system::ScriptHandler>().get());

    // the full capacity of the systems are used in parsing, so the systems must be configured and ready to go!
    WorldIO world( context_ );
    world.read( "data/scene.json", entities_, events_ );

    keyboard_.registerKeyDownCallback( Keycode::KeyF1,
      [this]() -> void {
          auto ui = this->systems_.system< system::Ui >();
          ui->toggleDisplay();
      });
    keyboard_.registerKeyDownCallback( Keycode::KeyP,
        [this]() -> void {
          this->requestStackPush_( states::Pause );
      });

    mouse_.registerMouseDownCallback(MouseButton::Left,
        [this]() -> void {
        auto pick = this->systems_.system<system::PickingSystem>();
        auto coords = mouse_.getMouseCoords();
        ecs::Entity target = pick->rayCast(entities_, events_, coords.x, coords.y);
        if (target.isValid()) {
            LOG_DEBUG << "HIT";
        }
        else {
            LOG_DEBUG << "MISS";
        }
    });
}

bool GameState::update( float dt ) {
    keyboard_.handleKeyPressedCallbacks();
    mouse_.handleMousePressedCallbacks();
    systems_.update< system::ScriptHandler >( dt );
    return false;
}

bool GameState::handleEvent( const SDL_Event& event ) { 
    if ( event.type == SDL_QUIT ) {
        requestStackClear_();
        context_.running = false;
    }
    keyboard_.handleEvent(event);
    mouse_.handleEvent(event);
    return false; 
}

void GameState::render( float dt ) {
    systems_.update< system::Renderer >( dt );
    systems_.update<system::DebugRenderSystem>(dt);
    systems_.update< system::Ui >( dt );
}

}
