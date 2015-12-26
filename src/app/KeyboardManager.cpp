#include "app/KeyboardManager.h"
#include "utils/Log.h"
#include <cstdint>

namespace pg {

void KeyboardManager::handleEvent( const SDL_Event& event ) {
    
    if ( event.type == SDL_KEYDOWN ) {
        auto it = keyDownCommands_.find( int (event.key.keysym.sym) );
        if ( it != keyDownCommands_.end() ) {
            it->second.execute();
        }
    } else if ( event.type == SDL_KEYUP ) {
        auto it = keyUpCommands_.find( int (event.key.keysym.sym) );
        if ( it != keyUpCommands_.end() ) {
            it->second.execute();
        }
    }
}

void KeyboardManager::handleKeyPressedCommands() {
    // handle real time input
    const uint8_t* state = SDL_GetKeyboardState( NULL );
    for ( auto& c: keyPressedCommands_ ) {
        // the key code is the index
        if ( state[ SDL_GetScancodeFromKey( int (c.first) ) ] ) {
            c.second.execute();
        }
    }
}

void KeyboardManager::registerKeyDownCommand( Keycode code, const Command& command ) {
    keyDownCommands_.emplace( int (code), command );
}

void KeyboardManager::registerKeyPressedCommand( Keycode code, const Command& command ) {
    keyPressedCommands_.emplace_back( std::make_pair( int (code), command ) );
}

void KeyboardManager::registerKeyUpCommand( Keycode code, const Command& command ) {
    keyUpCommands_.emplace( int (code), command );
}

}
