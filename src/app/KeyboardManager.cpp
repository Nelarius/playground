#include "app/KeyboardManager.h"
#include "utils/Log.h"
#include <SDL2/SDL_events.h>
#include <cstdint>

namespace pg {

void KeyboardManager::addInput( SDL_Scancode code, const Command& command ) {
    commands_.emplace_back( std::make_pair( code, command ) );
}

void KeyboardManager::handleEvent( const SDL_Event& event ) {
    
    if ( event.type == SDL_KEYDOWN ) {
        auto it = keyDownCommands_.find( event.key.keysym.sym );
        if ( it != keyDownCommands_.end() ) {
            it->second.execute();
        }
    } else if ( event.type == SDL_KEYUP ) {
        auto it = keyUpCommands_.find( event.key.keysym.sym );
        if ( it != keyUpCommands_.end() ) {
            it->second.execute();
        }
    }
    
    // handle real time input
    const uint8_t* state = SDL_GetKeyboardState( NULL );
    for ( auto& c: keyPressedCommands_ ) {
        // the key code is the index
        if ( state[ SDL_GetScancodeFromKey( c.first ) ] ) {
            c.second.execute();
        }
    }
}

void KeyboardManager::registerKeyDownCommand( SDL_Keycode code, const Command& command ) {
    keyDownCommands_.emplace( code, command );
}

void KeyboardManager::registerKeyPressedCommand( SDL_Keycode code, const Command& command ) {
    keyPressedCommands_.emplace_back( std::make_pair( code, command ) );
}

void KeyboardManager::registerKeyUpCommand( SDL_Keycode code, const Command& command ) {
    keyUpCommands_.emplace( code, command );
}

}
