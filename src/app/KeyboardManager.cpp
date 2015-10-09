#include "app/KeyboardManager.h"
#include <cstdint>

namespace pg {

void KeyboardManager::addInput( SDL_Scancode code, const Command& command ) {
    commands_.emplace_back( std::make_pair( code, command ) );
}

void KeyboardManager::handleInput() {
    const uint8_t* state = SDL_GetKeyboardState( NULL );
    for ( auto& c: commands_ ) {
        if ( state[ c.first ] ) {
            c.second.execute();
        }
    }
}

}
